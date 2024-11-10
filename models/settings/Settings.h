/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_SETTINGS_H
#define TOS_MODELS_SETTINGS_H

#include "../../stdlib/Types.h"
#include "../../gpuapi/AntiAliasing.h"
#include "../chat/ChatStatus.h"
#include "setting_types.h"
#include "../../module/Module.h"

#if _WIN32
    #include <windows.h>
    #include "../../platform/win32/input/controller/ControllerHandler.h"
#else __linux__
    #include <linux/limits.h>
    #define MAX_PATH PATH_MAX
#endif

#ifndef RENDER_CHUNK_RADIUS
    #define RENDER_CHUNK_RADIUS 10
#endif

#ifndef RENDER_BLOCK_OBJECT_CHUNK_RADIUS
    #define RENDER_BLOCK_OBJECT_CHUNK_RADIUS 10
#endif

#ifndef RENDER_INTERACTIVE_CHUNK_RADIUS
    #define RENDER_INTERACTIVE_CHUNK_RADIUS 1
#endif

#ifndef RENDER_OBJECT_CHUNK_RADIUS
    #define RENDER_OBJECT_CHUNK_RADIUS 1
#endif

#ifndef RENDER_MONSTER_CHUNK_RADIUS
    #define RENDER_MONSTER_CHUNK_RADIUS 3
#endif

#ifndef RENDER_NPC_CHUNK_RADIUS
    #define RENDER_NPC_CHUNK_RADIUS 3
#endif

#ifndef RENDER_PAYER_CHUNK_RADIUS
    #define RENDER_PAYER_CHUNK_RADIUS 3
#endif

#define MAX_ACTIVE_EXTENSIONS 15

// @todo remove default values because we will load them during startup
struct SSettings {
    char path[MAX_PATH];
    bool is_changed = false;
    byte simd_version;

    char network_hostname[64];
    uint16 network_port;

    byte distance_terrain = RENDER_CHUNK_RADIUS;
    byte distance_terrain_secondary = RENDER_BLOCK_OBJECT_CHUNK_RADIUS;
    byte distance_terrain_tertiary = RENDER_INTERACTIVE_CHUNK_RADIUS;
    byte distance_models = RENDER_OBJECT_CHUNK_RADIUS;
    byte distance_monster = RENDER_MONSTER_CHUNK_RADIUS;
    byte distance_npc = RENDER_NPC_CHUNK_RADIUS;
    byte distance_player = RENDER_PAYER_CHUNK_RADIUS;

    uint32 cache_player = 8192; // = max active players on a server
    uint32 cache_monster = 8192;
    uint32 cache_npc = 8192;
    uint32 cache_guild = 128;
    uint32 cache_message = 1024;

    uint32 interpolation_buffer;

    bool is_auction_house_enabled = true;
    bool is_direct_trading_enabled = true;

    // @todo add more server settings for tournaments, tournament modes
    // @todo add more server settings for raids and dungeons
    // @todo add more settings for pvp
};

// Player settings that the server needs to know about
struct PSettings {
    byte render_distance_models = RENDER_OBJECT_CHUNK_RADIUS;
    byte render_distance_monster = RENDER_MONSTER_CHUNK_RADIUS;
    byte render_distance_npc = RENDER_NPC_CHUNK_RADIUS;
    byte render_distance_player = RENDER_PAYER_CHUNK_RADIUS;

    byte chat_status = CHAT_STATUS_OFFLINE;
    bool allow_invites = true;
};

struct CSettings {
    // Evaluated during startup
    char path[MAX_PATH];
    bool is_changed = false;
    byte simd_version;
    int32 simd_step_size;
    bool supports_abm;

    // Network data
    char network_hostname[64];
    uint16 network_port;

    byte gpu_api = SETTING_TYPE_GPU_API_NONE;
    byte gpu_type = SETTING_TYPE_GPU_MEDIUM;
    byte gpu_fps = SETTING_TYPE_UNLIMITED;
    byte gpu_memory; // how much vram are we using on the gpu
    byte system_ram; // how much ram are we using
    byte system_threads; // how much ram are we using
    byte system_cache; // how much hard drive cache do we want to allow? (oldest files will be deleted)

    f32 gpu_aspect_ratio;
    byte gpu_brightness;
    byte gpu_contrast;
    byte gpu_gamma;
    f32 gpu_fov;
    int8 gpu_sync;
    AntiAliasingType gpu_aa_type;
    int8 gpu_aa_samples;

    byte gpu_render_distance_terrain = 10;
    byte gpu_render_distance_terrain_secondary = 10;
    byte gpu_render_distance_terrain_tertiary = 1;
    byte gpu_render_distance_models = 1;
    byte gpu_render_distance_monster = 3;
    byte gpu_render_distance_npc = 3;
    byte gpu_render_distance_player = 3;

    int32 gpu_render_count_mob = 1000;

    uint32 player_cache = 512;
    uint32 monster_cache = 128;
    uint32 npc_cache = 128;
    uint32 guild_cache = 128;
    uint32 message_cache = 64;

    byte gpu_animation_quality;
    byte gpu_attack_effect_quality;
    byte gpu_shadow_quality;
    byte gpu_terrain_quality;
    byte gpu_water_quality;
    byte gpu_grass_density;
    byte gpu_model_quality;
    byte gpu_texture_quality;
    byte gpu_foliage_distance;
    byte gpu_detail_level;
    byte gpu_reflection_quality;
    byte gpu_refraction_quality;
    byte gpu_caustics_quality;
    byte gpu_footprint_quality; // mostly used for snow, sand mud
    bool gpu_screen_effects; // e.g. water droplets/dust/freezing on screen

    bool gpu_raytracing = false;
    bool gpu_lense_effect = true;
    bool gpu_fog_effect = true;
    bool gpu_particles_environment = true;
    bool gpu_particles_players = true;
    bool gpu_particles_monster = true;
    bool gpu_particles_ui = true;
    bool gpu_particles_skills = true;
    bool gpu_particles_weapons = true;

    byte gpu_shadow_type = SETTING_TYPE_DISABLED; // none, baked, shadow mapping, point shadow, ray tracing
    byte gpu_light_ssao = SETTING_TYPE_DISABLED;
    byte gpu_light_bloom = SETTING_TYPE_DISABLED;

    byte gpu_reflection_blur = SETTING_TYPE_DISABLED;
    byte gpu_motion_blur = SETTING_TYPE_DISABLED;
    byte gpu_blur = SETTING_TYPE_DISABLED;
    byte gpu_anti_aliasing = SETTING_TYPE_DISABLED;
    byte gpu_anti_aliasing_detail = 0;
    byte gpu_sharpening = SETTING_TYPE_DISABLED;
    byte gpu_ambient_occlusion = SETTING_TYPE_DISABLED;
    byte gpu_color_deficiency;

    bool gpu_gamma_correction = true;
    bool gpu_normal_mapping = true;
    bool gpu_parallax_mapping = true;

    bool gpu_depth_of_field = true;
    bool gpu_chromatic_aberration = true;
    bool gpu_vignetting = true;
    bool gpu_light_shafts = true;

    byte audio_volume_master = 128;
    byte audio_volume_game = 128;
    byte audio_volume_environment = 128;
    byte audio_volume_music = 128;
    byte audio_volume_speech = 128;

    uint16 game_window1_dim[2];
    uint16 game_window1_pos[2];
    byte game_window1_mode = SETTING_TYPE_WINDOW_MODE_FULLSCREEN;

    byte game_view = SETTING_TYPE_PERSPECTIVE_FIRST;
    byte game_camera_zoom;
    bool game_camera_shake = false;

    // General game UI
    byte game_crosshair;

    uint32 game_interpolation_buffer;

    bool game_player_chat = false; // bubble above player
    byte game_chat_status = CHAT_STATUS_OFFLINE;

    // Extra windows
    byte game_window2_type = SETTING_TYPE_DISABLED;
    bool game_window2_visible = false;
    uint32 game_window2_dim[2] = {1024, 768};
    uint32 game_window2_pos[2];

    byte game_window3_type = SETTING_TYPE_DISABLED;
    bool game_window3_visible = false;
    uint32 game_window3_dim[2] = {1024, 768};
    uint32 game_window3_pos[2];

    byte game_window4_type = SETTING_TYPE_DISABLED;
    bool game_window4_visible = false;
    uint32 game_window4_dim[2] = {1024, 768};
    uint32 game_window4_pos[2];

    byte game_window5_type = SETTING_TYPE_DISABLED;
    bool game_window5_visible = false;
    uint32 game_window5_dim[2] = {1024, 768};
    uint32 game_window5_pos[2];

    // @todo Consider to allow settings for chat tabs
    //  define which messags go to which tab
    //  define custom chat tabs

    // UI settings
    // Themes
    char game_language[2];

    char game_theme[32];

    v4_f32 game_ui_dim[50];

    // @todo replace settings below with bit flag
    // UI
    uint64 ui_visibility_flags = 0;
    uint64 game_visibility_flags = 0;
    uint64 debug_visibility_flags = 0;

    // HUD
    bool game_hud_animated;

    bool game_show_buffs = false;

    byte game_minion_visibility_self = 128;
    byte game_minion_visibility_player = 128;

    bool game_minimap_show_merchants = false;
    bool game_minimap_show_quest = false;
    bool game_minimap_show_dungeons = false;
    bool game_minimap_show_names = false;
    bool game_show_clock = false;

    bool game_map_show_merchants = false;
    bool game_map_show_quest = false;
    bool game_map_show_dungeons = false;
    bool game_map_show_names = false;

    // Mounts
    uint32 game_default_mount = 0;

    // Game behavior
    bool game_error_audio = true;
    bool game_error_text = true;
    bool game_block_trade = false;
    bool game_block_group_invite = false;
    bool game_block_guild_invite = false;
    bool game_block_chat_invite = false;
    bool game_block_friend_invite = false;
    bool game_automatically_track_newest_quest = false;

    byte game_interact_radius = 1;

    // Game pad settings
    byte input_device_types = SETTING_INPUT_DEVICE_TYPE_MOUSE_KEYBOARD;
    byte input_controller_handler = CONTROLLER_HANDLER_TYPE_AUTO;
    byte stick_left_deadzone = 0;
    byte stick_right_deadzone = 0;

    f32 input_camera_speed;
    f32 input_look_sensitivity;
    bool input_invert_mouse = false;

    int32 active_module_count;
    Module* active_modules;

    // Hotkey settings
    // @todo hotkey combination e.g. alt+1
    byte hotkeys_movement_up = 0x57; // W
    byte hotkeys_movement_down = 0x53; // S
    byte hotkeys_movement_left = 0x41; // A
    byte hotkeys_movement_right = 0x44; // D

    byte hotkeys_cancel_action = 0x44; // X

    byte hotkeys_skill_tab_1 = 0x70; // F1
    byte hotkeys_skill_tab_2 = 0x71; // F2
    byte hotkeys_skill_tab_3 = 0x72; // F3

    byte hotkeys_skill_1 = 0x31; // 1
    byte hotkeys_skill_2 = 0x32; // 2
    byte hotkeys_skill_3 = 0x33; // 3
    byte hotkeys_skill_4 = 0x34; // 4
    byte hotkeys_skill_5 = 0x35; // 5
    byte hotkeys_skill_6 = 0x36; // 6
    byte hotkeys_skill_7 = 0x37; // 7
    byte hotkeys_skill_8 = 0x38; // 8
    byte hotkeys_skill_9 = 0x39; // 9
    byte hotkeys_skill_10 = 0x30; // 0

    byte hotkeys_interact = 45; // E
    byte hotkeys_jump = 0x20; // SPACE

    byte hotkeys_dodge = 0x20; // SPACE
    byte hotkeys_crouch = 0x14; // CAP
    byte hotkeys_walk = 0x11; // CTRL (switches to walking speed, one click only)
    byte hotkeys_emote = 0x12; // LEFT_ALT
    byte hotkeys_vertical_up = 0x14; // CAPS LOCK
    byte hotkeys_vertical_down = 0x10; // SHIFT

    byte hotkeys_view_next = 0x09; // TAB
    byte hotkeys_view_prv = 0x14; // CAPS LOCK
    byte hotkeys_compare_item = 0x11; // CTRL

    byte hotkeys_inventory = 0x49; // I
    byte hotkeys_character = 0x43; // C
    byte hotkeys_skills = 0x53; // S
    byte hotkeys_map = 0x4D; // M
    byte hotkeys_quest = 0x51; // Q
    byte hotkeys_attack_move = 0x52; // R
    byte hotkeys_force_move = 0x46; // F
    byte hotkeys_courser_move = 0x58; // X (move to where courser is)

    byte hotkeys_chat = 0x0D; // ENTER
    byte hotkeys_toggle_hud = 0x48; // H
    byte hotkeys_close_active_window = 0x48; // H
    byte hotkeys_toggle_all_windows = 0x48; // H

    byte hotkeys_zoom_in = 0x21; // Page up (@todo make mouse scroll up)
    byte hotkeys_zoom_out = 0x22; // page down (@todo make mouse scroll down)
    byte hotkeys_camera_look = 0x00; // @todo make right mouse hold
    byte hotkeys_camera_fly_mode = 0x00; // @todo make right mouse hold

    byte hotkeys_menu = 0x1B; // ESC

    byte hotkeys_marker_1 = 0x31;
    byte hotkeys_marker_2 = 0x32;
    byte hotkeys_marker_3 = 0x33;
    byte hotkeys_marker_4 = 0x34;
    byte hotkeys_marker_5 = 0x35;
    byte hotkeys_marker_6 = 0x36;
    byte hotkeys_ping = 0x37;

    // Camera settings/positions
    // Makes it easy to switch to customizable camera positions
    byte hotkeys_camera_1 = 0x0;
    byte hotkeys_camera_2 = 0x0;
    byte hotkeys_camera_3 = 0x0;

    char modules[MAX_ACTIVE_EXTENSIONS * 32];
};

void load_settings(CSettings* __restrict client_settings, char* data)
{
    char* pos = data;
    char* name;

    while (*pos != '\0') {
        // Skip all whitespaces and new lines
        int32 skip;
        while ((skip = (int32) is_whitespace(*pos)) || (skip = is_eol(pos))) {
            pos += skip;
        }

        // Skip comment
        if (*pos == '/' && pos[1] == '/') {
            while (*pos != '\n' && *pos != '\0') {
                ++pos;
            }

            continue;
        }

        // Get name
        name = pos;
        while (!is_eol(pos) && *pos != '\0' && !is_whitespace(*pos) && *pos != '[') {
            ++pos;
        }

        // Move to value
        while (is_whitespace(*pos)) {
            ++pos;
        }

        // Is there a value?
        if (is_eol(pos) || *pos == '\0') {
            continue;
        }

        // Parse value
        // We use grouping for faster handling
        if (strncmp(name, "gpu", sizeof("gpu") - 1) == 0) {
            name += sizeof("gpu") - 1;

            if (strncmp(name, "_ambient_occlusion", sizeof("_ambient_occlusion") - 1) == 0) {
            } else if (strncmp(name, "_animation_quality", sizeof("_animation_quality") - 1) == 0) {
            } else if (strncmp(name, "_anti_aliasing_detail", sizeof("_anti_aliasing_detail") - 1) == 0) {
            } else if (strncmp(name, "_anti_aliasing", sizeof("_anti_aliasing") - 1) == 0) {
            } else if (strncmp(name, "_api", sizeof("_api") - 1) == 0) {
            } else if (strncmp(name, "_aspect_ratio", sizeof("_aspect_ratio") - 1) == 0) {
                client_settings->gpu_aspect_ratio = (f32) atof(pos);
            } else if (strncmp(name, "_attack_effect_quality", sizeof("_attack_effect_quality") - 1) == 0) {
            } else if (strncmp(name, "_blur", sizeof("_blur") - 1) == 0) {
            } else if (strncmp(name, "_color_deficiency", sizeof("_color_deficiency") - 1) == 0) {
                client_settings->gpu_color_deficiency = (byte) atoi(pos);
            } else if (strncmp(name, "_brightness", sizeof("_brightness") - 1) == 0) {
            } else if (strncmp(name, "_camera_shake", sizeof("_camera_shake") - 1) == 0) {
            } else if (strncmp(name, "_caustics_quality", sizeof("_caustics_quality") - 1) == 0) {
            } else if (strncmp(name, "_chromatic_aberration", sizeof("_chromatic_aberration") - 1) == 0) {
            } else if (strncmp(name, "_contrast", sizeof("_contrast") - 1) == 0) {
            } else if (strncmp(name, "_depth_of_field", sizeof("_depth_of_field") - 1) == 0) {
            } else if (strncmp(name, "_detail_level", sizeof("_detail_level") - 1) == 0) {
            } else if (strncmp(name, "_fog_effect", sizeof("_fog_effect") - 1) == 0) {
            } else if (strncmp(name, "_foliage_distance", sizeof("_foliage_distance") - 1) == 0) {
            } else if (strncmp(name, "_footprint_quality", sizeof("_footprint_quality") - 1) == 0) {
            } else if (strncmp(name, "_fov", sizeof("_fov") - 1) == 0) {
                client_settings->gpu_fov = (f32) atof(pos);
            } else if (strncmp(name, "_fps", sizeof("_fps") - 1) == 0) {
            } else if (strncmp(name, "_gamma_correction", sizeof("_gamma_correction") - 1) == 0) {
            } else if (strncmp(name, "_gamma", sizeof("_gamma") - 1) == 0) {
            } else if (strncmp(name, "_grass_density", sizeof("_grass_density") - 1) == 0) {
            } else if (strncmp(name, "_lense_effect", sizeof("_lense_effect") - 1) == 0) {
            } else if (strncmp(name, "_light_bloom", sizeof("_light_bloom") - 1) == 0) {
            } else if (strncmp(name, "_light_shafts", sizeof("_light_shafts") - 1) == 0) {
            } else if (strncmp(name, "_light_ssao", sizeof("_light_ssao") - 1) == 0) {
            } else if (strncmp(name, "_memory", sizeof("_memory") - 1) == 0) {
            } else if (strncmp(name, "_model_quality", sizeof("_model_quality") - 1) == 0) {
            } else if (strncmp(name, "_motion_blur", sizeof("_motion_blur") - 1) == 0) {
            } else if (strncmp(name, "_normal_mapping", sizeof("_normal_mapping") - 1) == 0) {
            } else if (strncmp(name, "_parallax_mapping", sizeof("_parallax_mapping") - 1) == 0) {
            } else if (strncmp(name, "_particles_environment", sizeof("_particles_environment") - 1) == 0) {
            } else if (strncmp(name, "_particles_monster", sizeof("_particles_monster") - 1) == 0) {
            } else if (strncmp(name, "_particles_players", sizeof("_particles_players") - 1) == 0) {
            } else if (strncmp(name, "_particles_skills", sizeof("_particles_skills") - 1) == 0) {
            } else if (strncmp(name, "_particles_ui", sizeof("_particles_ui") - 1) == 0) {
            } else if (strncmp(name, "_particles_weapons", sizeof("_particles_weapons") - 1) == 0) {
            } else if (strncmp(name, "_raytracing", sizeof("_raytracing") - 1) == 0) {
            } else if (strncmp(name, "_reflection_blur", sizeof("_reflection_blur") - 1) == 0) {
            } else if (strncmp(name, "_reflection_quality", sizeof("_reflection_quality") - 1) == 0) {
            } else if (strncmp(name, "_refraction_quality", sizeof("_refraction_quality") - 1) == 0) {
            } else if (strncmp(name, "_render_count_mob", sizeof("_render_count_mob") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_models", sizeof("_render_distance_models") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_monster", sizeof("_render_distance_monster") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_npc", sizeof("_render_distance_npc") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_player", sizeof("_render_distance_player") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_terrain_secondary", sizeof("_render_distance_terrain_secondary") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_terrain_tertiary", sizeof("_render_distance_terrain_tertiary") - 1) == 0) {
            } else if (strncmp(name, "_render_distance_terrain", sizeof("_render_distance_terrain") - 1) == 0) {
            } else if (strncmp(name, "_screen_effects", sizeof("_screen_effects") - 1) == 0) {
            } else if (strncmp(name, "_shadow_quality", sizeof("_shadow_quality") - 1) == 0) {
            } else if (strncmp(name, "_shadow_type", sizeof("_shadow_type") - 1) == 0) {
            } else if (strncmp(name, "_sharpening", sizeof("_sharpening") - 1) == 0) {
            } else if (strncmp(name, "_sync", sizeof("_sync") - 1) == 0) {
                client_settings->gpu_sync = (int8) atoi(pos);
            } else if (strncmp(name, "_aa_type", sizeof("_aa_type") - 1) == 0) {
                client_settings->gpu_aa_type = (AntiAliasingType) atoi(pos);
            } else if (strncmp(name, "_aa_samples", sizeof("_aa_samples") - 1) == 0) {
                client_settings->gpu_aa_samples = (int8) atoi(pos);
            } else if (strncmp(name, "_terrain_quality", sizeof("_terrain_quality") - 1) == 0) {
            } else if (strncmp(name, "_texture_quality", sizeof("_texture_quality") - 1) == 0) {
            } else if (strncmp(name, "_type", sizeof("_type") - 1) == 0) {
            } else if (strncmp(name, "_vignetting", sizeof("_vignetting") - 1) == 0) {
            } else if (strncmp(name, "_water_quality", sizeof("_water_quality") - 1) == 0) {
            }
        } else if (strncmp(name, "game", sizeof("game") - 1) == 0) {
            name += sizeof("game") - 1;

            if (strncmp(name, "_automatically_track_newest_quest", sizeof("_automatically_track_newest_quest") - 1) == 0) {
            } else if (strncmp(name, "_block_chat_invite", sizeof("_block_chat_invite") - 1) == 0) {
            } else if (strncmp(name, "_block_friend_invite", sizeof("_block_friend_invite") - 1) == 0) {
            } else if (strncmp(name, "_block_group_invite", sizeof("_block_group_invite") - 1) == 0) {
            } else if (strncmp(name, "_block_guild_invite", sizeof("_block_guild_invite") - 1) == 0) {
            } else if (strncmp(name, "_block_trade", sizeof("_block_trade") - 1) == 0) {
            } else if (strncmp(name, "_camera_shake", sizeof("_camera_shake") - 1) == 0) {
            } else if (strncmp(name, "_camera_zoom", sizeof("_camera_zoom") - 1) == 0) {
            } else if (strncmp(name, "_chat_status", sizeof("_chat_status") - 1) == 0) {
            } else if (strncmp(name, "_crosshair", sizeof("_crosshair") - 1) == 0) {
            } else if (strncmp(name, "_default_mount", sizeof("_default_mount") - 1) == 0) {
            } else if (strncmp(name, "_hud_animated", sizeof("_hud_animated") - 1) == 0) {
            } else if (strncmp(name, "_interact_radius", sizeof("_interact_radius") - 1) == 0) {
            } else if (strncmp(name, "_language", sizeof("_language") - 1) == 0) {
            } else if (strncmp(name, "_map_show_dungeons", sizeof("_map_show_dungeons") - 1) == 0) {
            } else if (strncmp(name, "_map_show_merchants", sizeof("_map_show_merchants") - 1) == 0) {
            } else if (strncmp(name, "_map_show_names", sizeof("_map_show_names") - 1) == 0) {
            } else if (strncmp(name, "_map_show_quest", sizeof("_map_show_quest") - 1) == 0) {
            } else if (strncmp(name, "_minimap_show_dungeons", sizeof("_minimap_show_dungeons") - 1) == 0) {
            } else if (strncmp(name, "_minimap_show_merchants", sizeof("_minimap_show_merchants") - 1) == 0) {
            } else if (strncmp(name, "_minimap_show_names", sizeof("_minimap_show_names") - 1) == 0) {
            } else if (strncmp(name, "_minimap_show_quest", sizeof("_minimap_show_quest") - 1) == 0) {
            } else if (strncmp(name, "_minion_visibility_player", sizeof("_minion_visibility_player") - 1) == 0) {
            } else if (strncmp(name, "_minion_visibility_self", sizeof("_minion_visibility_self") - 1) == 0) {
            } else if (strncmp(name, "_player_chat", sizeof("_player_chat") - 1) == 0) {
            } else if (strncmp(name, "_show_buffs", sizeof("_show_buffs") - 1) == 0) {
            } else if (strncmp(name, "_show_cooldown_times", sizeof("_show_cooldown_times") - 1) == 0) {
            } else if (strncmp(name, "_show_dmg_numbers", sizeof("_show_dmg_numbers") - 1) == 0) {
            } else if (strncmp(name, "_show_dodge", sizeof("_show_dodge") - 1) == 0) {
            } else if (strncmp(name, "_show_effect_gains", sizeof("_show_effect_gains") - 1) == 0) {
            } else if (strncmp(name, "_show_health_bar_monster", sizeof("_show_health_bar_monster") - 1) == 0) {
            } else if (strncmp(name, "_show_health_bar_player", sizeof("_show_health_bar_player") - 1) == 0) {
            } else if (strncmp(name, "_show_health_bar_self", sizeof("_show_health_bar_self") - 1) == 0) {
            } else if (strncmp(name, "_show_health_numbers", sizeof("_show_health_numbers") - 1) == 0) {
            } else if (strncmp(name, "_show_name_monster", sizeof("_show_name_monster") - 1) == 0) {
            } else if (strncmp(name, "_show_name_npc", sizeof("_show_name_npc") - 1) == 0) {
            } else if (strncmp(name, "_show_name_player", sizeof("_show_name_player") - 1) == 0) {
            } else if (strncmp(name, "_show_name_self", sizeof("_show_name_self") - 1) == 0) {
            } else if (strncmp(name, "_show_resource_numbers", sizeof("_show_resource_numbers") - 1) == 0) {
            } else if (strncmp(name, "_show_subtitles", sizeof("_show_subtitles") - 1) == 0) {
            } else if (strncmp(name, "_show_title_other", sizeof("_show_title_other") - 1) == 0) {
            } else if (strncmp(name, "_show_title_self", sizeof("_show_title_self") - 1) == 0) {
            } else if (strncmp(name, "_show_xp_bar_numbers", sizeof("_show_xp_bar_numbers") - 1) == 0) {
            } else if (strncmp(name, "_ui_visibility_flags", sizeof("_ui_visibility_flags") - 1) == 0) {
                client_settings->ui_visibility_flags = strtoull(pos, &pos, 10);
            } else if (strncmp(name, "_visibility_flags", sizeof("_visibility_flags") - 1) == 0) {
                client_settings->game_visibility_flags = strtoull(pos, &pos, 10);
            } else if (strncmp(name, "_debug_visibility_flags", sizeof("_debug_visibility_flags") - 1) == 0) {
                client_settings->debug_visibility_flags = strtoull(pos, &pos, 10);
            } else if (strncmp(name, "_theme", sizeof("_theme") - 1) == 0) {
                pos += strcpy_to_eol(pos, client_settings->game_theme);
            } else if (strncmp(name, "_ui_dim", sizeof("_ui_dim") - 1) == 0) {
                int32 index = strtoul(++pos, &pos, 10);
                pos += 2;

                client_settings->game_ui_dim[index].x = strtof(pos, &pos); ++pos;
                client_settings->game_ui_dim[index].y = strtof(pos, &pos); ++pos;
                client_settings->game_ui_dim[index].width = strtof(pos, &pos); ++pos;
                client_settings->game_ui_dim[index].height = strtof(pos, &pos);
            } else if (strncmp(name, "_ui_show_hotkeys", sizeof("_ui_show_hotkeys") - 1) == 0) {
            } else if (strncmp(name, "_view", sizeof("_view") - 1) == 0) {
            } else if (strncmp(name, "_window1_dim", sizeof("_window1_dim") - 1) == 0) {
                client_settings->game_window1_dim[0] = (uint16) strtoul(pos, &pos, 10); ++pos;
                client_settings->game_window1_dim[1] = (uint16) atoi(pos);
            } else if (strncmp(name, "_window1_pos", sizeof("_window1_pos") - 1) == 0) {
            } else if (strncmp(name, "_window2_dim", sizeof("_window2_dim") - 1) == 0) {
            } else if (strncmp(name, "_window2_pos", sizeof("_window2_pos") - 1) == 0) {
            } else if (strncmp(name, "_window3_dim", sizeof("_window3_dim") - 1) == 0) {
            } else if (strncmp(name, "_window3_pos", sizeof("_window3_pos") - 1) == 0) {
            } else if (strncmp(name, "_window4_dim", sizeof("_window4_dim") - 1) == 0) {
            } else if (strncmp(name, "_window4_pos", sizeof("_window4_pos") - 1) == 0) {
            } else if (strncmp(name, "_window5_dim", sizeof("_window5_dim") - 1) == 0) {
            } else if (strncmp(name, "_window5_pos", sizeof("_window5_pos") - 1) == 0) {
            }
        } else if (strncmp(name, "hotkeys", sizeof("hotkeys") - 1) == 0) {
            name += sizeof("hotkeys") - 1;

            if (strncmp(name, "_camera_fly_mode", sizeof("_camera_fly_mode") - 1) == 0) {
            } else if (strncmp(name, "_camera_look", sizeof("_camera_look") - 1) == 0) {
            } else if (strncmp(name, "_zoom_in", sizeof("_zoom_in") - 1) == 0) {
            } else if (strncmp(name, "_zoom_out", sizeof("_zoom_out") - 1) == 0) {
            } else if (strncmp(name, "_attack_move", sizeof("_attack_move") - 1) == 0) {
            } else if (strncmp(name, "_auction_house", sizeof("_auction_house") - 1) == 0) {
            } else if (strncmp(name, "_bar_active_1", sizeof("_bar_active_1") - 1) == 0) {
            } else if (strncmp(name, "_bar_active_2", sizeof("_bar_active_2") - 1) == 0) {
            } else if (strncmp(name, "_bar_active_3", sizeof("_bar_active_3") - 1) == 0) {
            } else if (strncmp(name, "_bar_active_4", sizeof("_bar_active_4") - 1) == 0) {
            } else if (strncmp(name, "_bar_active_5", sizeof("_bar_active_5") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_1", sizeof("_bar_global_1") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_10", sizeof("_bar_global_10") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_11", sizeof("_bar_global_11") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_12", sizeof("_bar_global_12") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_13", sizeof("_bar_global_13") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_14", sizeof("_bar_global_14") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_15", sizeof("_bar_global_15") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_16", sizeof("_bar_global_16") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_17", sizeof("_bar_global_17") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_18", sizeof("_bar_global_18") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_19", sizeof("_bar_global_19") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_2", sizeof("_bar_global_2") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_20", sizeof("_bar_global_20") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_21", sizeof("_bar_global_21") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_22", sizeof("_bar_global_22") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_23", sizeof("_bar_global_23") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_24", sizeof("_bar_global_24") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_25", sizeof("_bar_global_25") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_3", sizeof("_bar_global_3") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_4", sizeof("_bar_global_4") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_5", sizeof("_bar_global_5") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_6", sizeof("_bar_global_6") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_7", sizeof("_bar_global_7") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_8", sizeof("_bar_global_8") - 1) == 0) {
            } else if (strncmp(name, "_bar_global_9", sizeof("_bar_global_9") - 1) == 0) {
            } else if (strncmp(name, "_bar_tab_1", sizeof("_bar_tab_1") - 1) == 0) {
            } else if (strncmp(name, "_bar_tab_2", sizeof("_bar_tab_2") - 1) == 0) {
            } else if (strncmp(name, "_bar_tab_3", sizeof("_bar_tab_3") - 1) == 0) {
            } else if (strncmp(name, "_bar_tab_4", sizeof("_bar_tab_4") - 1) == 0) {
            } else if (strncmp(name, "_bar_tab_5", sizeof("_bar_tab_5") - 1) == 0) {
            } else if (strncmp(name, "_camera_1", sizeof("_camera_1") - 1) == 0) {
            } else if (strncmp(name, "_camera_2", sizeof("_camera_2") - 1) == 0) {
            } else if (strncmp(name, "_camera_3", sizeof("_camera_3") - 1) == 0) {
            } else if (strncmp(name, "_cancel_action", sizeof("_cancel_action") - 1) == 0) {
            } else if (strncmp(name, "_character", sizeof("_character") - 1) == 0) {
            } else if (strncmp(name, "_chat_send", sizeof("_chat_send") - 1) == 0) {
            } else if (strncmp(name, "_close_active_window", sizeof("_close_active_window") - 1) == 0) {
            } else if (strncmp(name, "_compare_item", sizeof("_compare_item") - 1) == 0) {
            } else if (strncmp(name, "_courser_move", sizeof("_courser_move") - 1) == 0) {
            } else if (strncmp(name, "_crouch", sizeof("_crouch") - 1) == 0) {
            } else if (strncmp(name, "_dodge", sizeof("_dodge") - 1) == 0) {
            } else if (strncmp(name, "_element_down", sizeof("_element_down") - 1) == 0) {
            } else if (strncmp(name, "_element_left", sizeof("_element_left") - 1) == 0) {
            } else if (strncmp(name, "_element_next", sizeof("_element_next") - 1) == 0) {
            } else if (strncmp(name, "_element_prev", sizeof("_element_prev") - 1) == 0) {
            } else if (strncmp(name, "_element_right", sizeof("_element_right") - 1) == 0) {
            } else if (strncmp(name, "_element_up", sizeof("_element_up") - 1) == 0) {
            } else if (strncmp(name, "_emote", sizeof("_emote") - 1) == 0) {
            } else if (strncmp(name, "_force_move", sizeof("_force_move") - 1) == 0) {
            } else if (strncmp(name, "_guild", sizeof("_guild") - 1) == 0) {
            } else if (strncmp(name, "_interact", sizeof("_interact") - 1) == 0) {
            } else if (strncmp(name, "_inventory", sizeof("_inventory") - 1) == 0) {
            } else if (strncmp(name, "_jump", sizeof("_jump") - 1) == 0) {
            } else if (strncmp(name, "_map", sizeof("_map") - 1) == 0) {
            } else if (strncmp(name, "_marker_1", sizeof("_marker_1") - 1) == 0) {
            } else if (strncmp(name, "_marker_2", sizeof("_marker_2") - 1) == 0) {
            } else if (strncmp(name, "_marker_3", sizeof("_marker_3") - 1) == 0) {
            } else if (strncmp(name, "_marker_4", sizeof("_marker_4") - 1) == 0) {
            } else if (strncmp(name, "_marker_5", sizeof("_marker_5") - 1) == 0) {
            } else if (strncmp(name, "_marker_6", sizeof("_marker_6") - 1) == 0) {
            } else if (strncmp(name, "_menu", sizeof("_menu") - 1) == 0) {
            } else if (strncmp(name, "_movement_down", sizeof("_movement_down") - 1) == 0) {
            } else if (strncmp(name, "_movement_left", sizeof("_movement_left") - 1) == 0) {
            } else if (strncmp(name, "_movement_pitch_down", sizeof("_movement_pitch_down") - 1) == 0) {
            } else if (strncmp(name, "_movement_pitch_up", sizeof("_movement_pitch_up") - 1) == 0) {
            } else if (strncmp(name, "_movement_right", sizeof("_movement_right") - 1) == 0) {
            } else if (strncmp(name, "_movement_roll_left", sizeof("_movement_roll_left") - 1) == 0) {
            } else if (strncmp(name, "_movement_roll_right", sizeof("_movement_roll_right") - 1) == 0) {
            } else if (strncmp(name, "_movement_up", sizeof("_movement_up") - 1) == 0) {
            } else if (strncmp(name, "_movement_vertical_down", sizeof("_movement_vertical_down") - 1) == 0) {
            } else if (strncmp(name, "_movement_vertical_up", sizeof("_movement_vertical_up") - 1) == 0) {
            } else if (strncmp(name, "_movement_yaw_left", sizeof("_movement_yaw_left") - 1) == 0) {
            } else if (strncmp(name, "_movement_yaw_right", sizeof("_movement_yaw_right") - 1) == 0) {
            } else if (strncmp(name, "_ping", sizeof("_ping") - 1) == 0) {
            } else if (strncmp(name, "_quest", sizeof("_quest") - 1) == 0) {
            } else if (strncmp(name, "_skills", sizeof("_skills") - 1) == 0) {
            } else if (strncmp(name, "_sneak", sizeof("_sneak") - 1) == 0) {
            } else if (strncmp(name, "_toggle_all_windows", sizeof("_toggle_all_windows") - 1) == 0) {
            } else if (strncmp(name, "_toggle_hud", sizeof("_toggle_hud") - 1) == 0) {
            } else if (strncmp(name, "_view_next", sizeof("_view_next") - 1) == 0) {
            } else if (strncmp(name, "_view_prv", sizeof("_view_prv") - 1) == 0) {
            } else if (strncmp(name, "_walk", sizeof("_walk") - 1) == 0) {
            }
        } else if (strncmp(name, "input", sizeof("input") - 1) == 0) {
            name += sizeof("input") - 1;

            if (strncmp(name, "_camera_speed", sizeof("_camera_speed") - 1) == 0) {
                client_settings->input_camera_speed = (f32) atof(pos);
            } else if (strncmp(name, "_controller1_gyro_x_deadzone", sizeof("_controller1_gyro_x_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_controller1_gyro_x_zero", sizeof("_controller1_gyro_x_zero") - 1) == 0) {
            } else if (strncmp(name, "_controller1_gyro_y_deadzone", sizeof("_controller1_gyro_y_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_controller1_gyro_y_zero", sizeof("_controller1_gyro_y_zero") - 1) == 0) {
            } else if (strncmp(name, "_controller1_gyro_z_deadzone", sizeof("_controller1_gyro_z_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_controller1_gyro_z_zero", sizeof("_controller1_gyro_z_zero") - 1) == 0) {
            } else if (strncmp(name, "_controller1_stick_left_deadzone", sizeof("_controller1_stick_left_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_controller1_stick_right_deadzone", sizeof("_controller1_stick_right_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_controller1_trigger_left_deadzone", sizeof("_controller1_trigger_left_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_controller1_trigger_right_deadzone", sizeof("_controller1_trigger_right_deadzone") - 1) == 0) {
            } else if (strncmp(name, "_device", sizeof("_device") - 1) == 0) {
            } else if (strncmp(name, "_invert_mouse", sizeof("_invert_mouse") - 1) == 0) {
            } else if (strncmp(name, "_look_sensitivity", sizeof("_look_sensitivity") - 1) == 0) {
                client_settings->input_look_sensitivity = (f32) atof(pos);
            }
        } else if (strncmp(name, "cache", sizeof("cache") - 1) == 0) {
            name += sizeof("cache") - 1;

            if (strncmp(name, "_chat", sizeof("_chat") - 1) == 0) {
            } else if (strncmp(name, "_guild", sizeof("_guild") - 1) == 0) {
            } else if (strncmp(name, "_log", sizeof("_log") - 1) == 0) {
            } else if (strncmp(name, "_monster", sizeof("_monster") - 1) == 0) {
            } else if (strncmp(name, "_npc", sizeof("_npc") - 1) == 0) {
            } else if (strncmp(name, "_player", sizeof("_player") - 1) == 0) {
            }
        } else if (strncmp(name, "audio", sizeof("audio") - 1) == 0) {
            name += sizeof("audio") - 1;

            if (strncmp(name, "_volume_environment", sizeof("_volume_environment") - 1) == 0) {
            } else if (strncmp(name, "_volume_game", sizeof("_volume_game") - 1) == 0) {
            } else if (strncmp(name, "_volume_master", sizeof("_volume_master") - 1) == 0) {
            } else if (strncmp(name, "_volume_music", sizeof("_volume_music") - 1) == 0) {
            } else if (strncmp(name, "_volume_speech", sizeof("_volume_speech") - 1) == 0) {
            }
        } else if (strncmp(name, "network", sizeof("network") - 1) == 0) {
            name += sizeof("network") - 1;

            if (strncmp(name, "_hostname", sizeof("_hostname") - 1) == 0) {
            } else if (strncmp(name, "_port", sizeof("_port") - 1) == 0) {
            }
        } else if (strncmp(name, "system", sizeof("system") - 1) == 0) {
            name += sizeof("system") - 1;

            if (strncmp(name, "system_cache", sizeof("system_cache") - 1) == 0) {
            } else if (strncmp(name, "system_memory", sizeof("system_memory") - 1) == 0) {
            } else if (strncmp(name, "system_threads", sizeof("system_threads") - 1) == 0) {
            }
        } else {
            if (strncmp(name, "extension_active", sizeof("extension_active") - 1) == 0) {
            }
        }

        // Go to end of line
        while (*pos != '\n' && *pos != '\0') {
            ++pos;
        }
    }
}

#endif