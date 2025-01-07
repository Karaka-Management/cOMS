/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_APP_COMMAND_BUFFER_C
#define TOS_APP_COMMAND_BUFFER_C

/**
 * The AppCmdBuffer by itself doesn't do much, it simply takes in commands and executes them.
 * The actual execution depends on the implementation of the underlying systems like:
 *      ECS, AMS, AudioMixer, ...
 * The AppCmdBuffer simplifies the interaction with those systems since the caller has to care less
 * about the information flow, function structure etc.
 * On the other hand the caller loses some control:
 *      No control over the execution order, unless additional overhead like priority gets introduced
 *      No control over what type of command are executed, unless additional overhead like command type checks get introduced
 *      ...
 * In many cases you don't need this type of control, but when you need it you should probably look at how
 * this AppCmdBuffer interacts with the individual systems and manually call those
 */
#include "AppCmdBuffer.h"

inline
void cmd_buffer_create(AppCmdBuffer* cb, BufferMemory* buf, int32 commands_count)
{
    chunk_init(&cb->commands, buf, commands_count, sizeof(Command), 64);
    pthread_mutex_init(&cb->mutex, NULL);
}

// This doesn't load the asset directly but tells (most likely) a worker thread to load an asset
static inline
void cmd_asset_load_enqueue(AppCmdBuffer* cb, Command* cmd)
{
    queue_enqueue_wait_atomic(cb->assets_to_load, (byte *) cmd->data);
}

static inline
void* cmd_func_run(AppCmdBuffer* cb, Command* cmd)
{
    CommandFunc func = *((CommandFunc *) cmd->data);
    return func(cmd);
}

static inline
Asset* cmd_asset_load(AppCmdBuffer* cb, Command* cmd)
{
    int32 asset_id = (int32) str_to_int((char *) cmd->data);
    int32 archive_id = (asset_id >> 24) & 0xFF;
    return asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->thrd_mem_vol);
}

static inline
Asset* cmd_audio_play_enqueue(AppCmdBuffer* cb, Command* cmd)
{
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, (char *) cmd->data);
    if (!asset) {
        return asset;
    }

    // @todo How to handle settings = AudioInstance
    audio_mixer_play(
        &cb->mixer[(cmd->data + 32) ? *((int32 *) (cmd->data + 32)) : 0], // @bug how to handle multiple mixers
        asset->official_id + 1, // @bug + 1 necessary since it starts at 0, I think. we are still in the design phase :)
        (Audio *) asset->self
    );

    return asset;
}

static inline
Asset* cmd_audio_play_async(AppCmdBuffer* cb, Command* cmd)
{
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, (char *) cmd->data);
    if (!asset) {
        cmd_asset_load_enqueue(cb, cmd);
    } else {
        cmd_audio_play_enqueue(cb, cmd);
    }

    return asset;
}

static inline
Asset* cmd_texture_create(AppCmdBuffer* cb, Command* cmd)
{
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, (char *) cmd->data);
    if (!asset) {
        return asset;
    }

    Texture* texture = (Texture *) asset->self;
    if (cb->gpu_api == GPU_API_TYPE_OPENGL
        && !(texture->image.image_settings & IMAGE_SETTING_BOTTOM_TO_TOP)
    ) {
        image_flip_vertical(cb->thrd_mem_vol, &texture->image);
    }

    return asset;
}

static inline
Asset* cmd_texture_load_async(AppCmdBuffer* cb, Command* cmd)
{
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, (char *) cmd->data);
    if (!asset) {
        cmd_asset_load_enqueue(cb, cmd);
    } else {
        cmd_texture_create(cb, cmd);
    }

    return asset;
}

static inline
Asset* cmd_font_create(AppCmdBuffer* cb, Command* cmd)
{
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, (char *) cmd->data);
    if (!asset) {
        return asset;
    }

    Font* font = (Font *) asset->self;
    if (cb->gpu_api == GPU_API_TYPE_OPENGL) {
        font_invert_coordinates(font);
    }

    return asset;
}

static inline
Asset* cmd_font_load_async(AppCmdBuffer* cb, Command* cmd)
{
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, (char *) cmd->data);
    if (!asset) {
        cmd_asset_load_enqueue(cb, cmd);
    } else {
        cmd_font_create(cb, cmd);
    }

    return asset;
}

inline
void thrd_cmd_insert(AppCmdBuffer* cb, Command* cmd_temp)
{
    pthread_mutex_lock(&cb->mutex);
    int32 index = chunk_reserve(&cb->commands, 1);
    if (index < 0) {
        pthread_mutex_unlock(&cb->mutex);
        ASSERT_SIMPLE(false);

        return;
    }

    if (index > cb->last_element) {
        cb->last_element = index;
    }

    Command* cmd = (Command *) chunk_get_element(&cb->commands, index);
    memcpy(cmd, cmd_temp, sizeof(Command));
    pthread_mutex_unlock(&cb->mutex);
}

inline
void thrd_cmd_insert(AppCmdBuffer* cb, CommandType type, int32 data)
{
    Command cmd;
    cmd.type = type;
    *((int32 *) cmd.data) = data;

    thrd_cmd_insert(cb, &cmd);
}

inline
void thrd_cmd_insert(AppCmdBuffer* cb, CommandType type, const char* data)
{
    Command cmd;
    cmd.type = type;
    str_copy_short((char *) cmd.data, data);

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_func_insert(AppCmdBuffer* cb, CommandType type, CommandFunc* func) {
    Command cmd;
    cmd.type = CMD_FUNC_RUN;
    *((CommandFunc *) cmd.data) = *func;

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_audio_play(AppCmdBuffer* cb, int32 data) {
    Command cmd;
    cmd.type = CMD_AUDIO_PLAY;
    *((int32 *) cmd.data) = data;

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_audio_play(AppCmdBuffer* cb, const char* data) {
    Command cmd;
    cmd.type = CMD_AUDIO_PLAY;
    str_copy_short((char *) cmd.data, data);

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_func_run(AppCmdBuffer* cb, CommandFunc* func) {
    Command cmd;
    cmd.type = CMD_FUNC_RUN;
    *((CommandFunc *) cmd.data) = *func;

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_texture_load(AppCmdBuffer* cb, int32 data) {
    Command cmd;
    cmd.type = CMD_TEXTURE_LOAD;
    *((int32 *) cmd.data) = data;

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_texture_load(AppCmdBuffer* cb, const char* data) {
    Command cmd;
    cmd.type = CMD_TEXTURE_LOAD;
    str_copy_short((char *) cmd.data, data);

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_font_load(AppCmdBuffer* cb, int32 data) {
    Command cmd;
    cmd.type = CMD_FONT_LOAD;
    *((int32 *) cmd.data) = data;

    thrd_cmd_insert(cb, &cmd);
}

inline void thrd_cmd_font_load(AppCmdBuffer* cb, const char* data) {
    Command cmd;
    cmd.type = CMD_FONT_LOAD;
    str_copy_short((char *) cmd.data, data);

    thrd_cmd_insert(cb, &cmd);
}

inline Asset* cmd_asset_load(AppCmdBuffer* cb, int32 asset_id)
{
    int32 archive_id = (asset_id >> 24) & 0xFF;
    return asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
}

inline Asset* cmd_asset_load(AppCmdBuffer* cb, const char* asset_id_str)
{
    int32 asset_id = (int32) str_to_int(asset_id_str);
    int32 archive_id = (asset_id >> 24) & 0xFF;
    return asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
}

inline Asset* cmd_audio_play(AppCmdBuffer* cb, int32 asset_id)
{
    // Check if asset already loaded
    char id_str[9];
    int_to_hex(asset_id, id_str);

    Asset* asset = thrd_ams_get_asset_wait(cb->ams, id_str);

    // Load asset if not loaded
    if (!asset) {
        int32 archive_id = (asset_id >> 24) & 0xFF;
        asset = asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
    }

    // @todo How to handle settings = AudioInstance
    audio_mixer_play(
        &cb->mixer[0], // @bug how to handle multiple mixers
        asset->official_id + 1, // @bug + 1 necessary since it starts at 0, I think. we are still in the design phase :)
        (Audio *) asset->self
    );

    return asset;
}

inline Asset* cmd_audio_play(AppCmdBuffer* cb, const char* name) {
    // Check if asset already loaded
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, name);

    // Load asset if not loaded
    if (!asset) {
        int32 asset_id = (int32) hex_to_int(name);
        int32 archive_id = (asset_id >> 24) & 0xFF;
        asset = asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
    }

    // @todo How to handle settings = AudioInstance
    audio_mixer_play(
        &cb->mixer[0], // @bug how to handle multiple mixers
        asset->official_id + 1, // @bug + 1 necessary since it starts at 0, I think. we are still in the design phase :)
        (Audio *) asset->self
    );

    return asset;
}

inline void* cmd_func_run(AppCmdBuffer* cb, CommandFunc func) {
    return func(NULL);
}

inline Asset* cmd_texture_load(AppCmdBuffer* cb, int32 asset_id) {
    // Check if asset already loaded
    char id_str[9];
    int_to_hex(asset_id, id_str);

    Asset* asset = thrd_ams_get_asset_wait(cb->ams, id_str);

    // Load asset if not loaded
    if (!asset) {
        int32 archive_id = (asset_id >> 24) & 0xFF;
        asset = asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
    }

    // Setup basic texture
    Texture* texture = (Texture *) asset->self;
    if (cb->gpu_api == GPU_API_TYPE_OPENGL
        && !(texture->image.image_settings & IMAGE_SETTING_BOTTOM_TO_TOP)
    ) {
        image_flip_vertical(cb->mem_vol, &texture->image);
    }

    // @question What about texture upload?

    return asset;
}

inline Asset* cmd_texture_load(AppCmdBuffer* cb, const char* name) {
    // Check if asset already loaded
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, name);

    // Load asset if not loaded
    if (!asset) {
        int32 asset_id = (int32) hex_to_int(name);
        int32 archive_id = (asset_id >> 24) & 0xFF;
        asset = asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
    }

    // Setup basic texture
    Texture* texture = (Texture *) asset->self;
    if (cb->gpu_api == GPU_API_TYPE_OPENGL
        && !(texture->image.image_settings & IMAGE_SETTING_BOTTOM_TO_TOP)
    ) {
        image_flip_vertical(cb->mem_vol, &texture->image);
    }

    // @question What about texture upload?

    return asset;
}

inline Asset* cmd_font_load(AppCmdBuffer* cb, int32 asset_id) {
    // Check if asset already loaded
    char id_str[9];
    int_to_hex(asset_id, id_str);

    Asset* asset = thrd_ams_get_asset_wait(cb->ams, id_str);

    // Load asset if not loaded
    if (!asset) {
        int32 archive_id = (asset_id >> 24) & 0xFF;
        asset = asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
    }

    // Setup font
    Font* font = (Font *) asset->self;
    if (cb->gpu_api == GPU_API_TYPE_OPENGL) {
        font_invert_coordinates(font);
    }

    // @question What about also loading the font atlas

    return asset;
}

inline Asset* cmd_font_load(AppCmdBuffer* cb, const char* name) {
    // Check if asset already loaded
    Asset* asset = thrd_ams_get_asset_wait(cb->ams, name);

    // Load asset if not loaded
    if (!asset) {
        int32 asset_id = (int32) hex_to_int(name);
        int32 archive_id = (asset_id >> 24) & 0xFF;
        asset = asset_archive_asset_load(&cb->asset_archives[archive_id], asset_id, cb->ams, cb->mem_vol);
    }

    // Setup font
    Font* font = (Font *) asset->self;
    if (cb->gpu_api == GPU_API_TYPE_OPENGL) {
        font_invert_coordinates(font);
    }

    // @question What about also loading the font atlas

    return asset;
}

// @question In some cases we don't remove an element if it couldn't get completed
// Would it make more sense to remove it and add a new follow up command automatically in such cases?
// e.g. couldn't play audio since it isn't loaded -> queue for asset load -> queue for internal play
// I gues this only makes sense if we would switch to a queue
void cmd_iterate(AppCmdBuffer* cb)
{
    int32 last_element = 0;
    int32 chunk_id = 0;
    chunk_iterate_start(&cb->commands, chunk_id)
        Command* cmd = (Command *) chunk_get_element(&cb->commands, chunk_id);
        bool remove = true;

        switch (cmd->type) {
            case CMD_FUNC_RUN: {
                    cmd_func_run(cb, cmd);
                } break;
            case CMD_ASSET_ENQUEUE: {
                    cmd_asset_load_enqueue(cb, cmd);
                } break;
            case CMD_ASSET_LOAD: {
                    cmd_asset_load(cb, cmd);
                } break;
            case CMD_FILE_LOAD: {} break;
            case CMD_TEXTURE_LOAD: {
                    remove = cmd_texture_load_async(cb, cmd) != NULL;
                } break;
            case CMD_TEXTURE_CREATE: {
                    // Internal only
                    cmd_texture_create(cb, cmd);
                } break;
            case CMD_FONT_LOAD: {
                    remove = cmd_font_load_async(cb, cmd) != NULL;
                } break;
            case CMD_FONT_CREATE: {
                    // Internal only
                    cmd_font_create(cb, cmd);
                } break;
            case CMD_AUDIO_PLAY: {
                    cmd_audio_play_async(cb, cmd);
                } break;
            case CMD_AUDIO_ENQUEUE: {
                    // Internal only
                    remove = cmd_audio_play_enqueue(cb, cmd) != NULL;
                } break;
            case CMD_SHADER_LOAD: {
                    remove = cmd_shader_load(cb, cmd) != NULL;
                } break;
            default: {
                UNREACHABLE();
            }
        }

        if (!remove) {
            last_element = chunk_id;
            continue;
        }

        chunk_free_element(&cb->commands, free_index, bit_index);

        // @performance This adds some unnecessary overhead.
        // It would be much better, if we could define cb->last_element as the limit in the for loop
        if (chunk_id == cb->last_element) {
            break;
        }
    chunk_iterate_end;

    cb->last_element = last_element;
}

// @performance Locking the entire thing during the iteration is horribly slow, fix.
// Solution 1: Use Queue
// Solution 2: create a mask for the chunk->free which will be set (and only then locked) after everything is done
//              This has the risk that if it takes a long time we may run out of free indices for insert
//              This shouldn't happen since the command buffer shouldn't fill up in just 1-3 frames
void thrd_cmd_iterate(AppCmdBuffer* cb)
{
    pthread_mutex_lock(&cb->mutex);
    cmd_iterate(cb);
    pthread_mutex_unlock(&cb->mutex);
}

#endif