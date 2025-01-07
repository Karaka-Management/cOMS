/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_APP_CMD_BUFFER_H
#define TOS_GPUAPI_OPENGL_APP_CMD_BUFFER_H

#include "../../stdlib/Types.h"
#include "OpenglUtils.h"
#include "Shader.h"
#include "ShaderUtils.h"
#include "../ShaderType.h"
#include "../../asset/Asset.h"

void* cmd_shader_load(AppCmdBuffer* cb, Command* cmd) {
    return NULL;
}

void* cmd_shader_load(AppCmdBuffer* cb, Shader* shader, int32* shader_ids) {
    char asset_id[9];

    int32 shader_assets[SHADER_TYPE_SIZE];
    for (int32 i = 0; i < SHADER_TYPE_SIZE; ++i) {
        shader_assets[i] = -1;
    }

    for (int32 i = 0; i < SHADER_TYPE_SIZE; ++i) {
        if (!shader_ids[i]) {
            continue;
        } else if (shader_ids[i] < 0) {
            break;
        }

        // Load sub asset
        int_to_hex(shader_ids[i], asset_id);
        Asset* shader_asset = thrd_ams_get_asset_wait(cb->ams, asset_id);
        if (!shader_asset) {
            int32 archive_id = (shader_ids[i] >> 24) & 0xFF;
            shader_asset = asset_archive_asset_load(&cb->asset_archives[archive_id], shader_ids[i], cb->ams, cb->mem_vol);
        }

        // Make sub shader
        shader_assets[i] = shader_make(
            shader_type_index((ShaderType) (i + 1)),
            (char *) shader_asset->self,
            cb->mem_vol
        );

        shader_asset->state |= ASSET_STATE_RAM_GC;
        shader_asset->state |= ASSET_STATE_VRAM_GC;
    }

    // Make shader/program
    shader->id = program_make(
        shader_assets[0], shader_assets[1], shader_assets[2],
        cb->mem_vol
    );

    return NULL;
}

#endif