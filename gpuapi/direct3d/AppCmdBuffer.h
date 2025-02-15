/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECTX_APP_CMD_BUFFER_H
#define TOS_GPUAPI_DIRECTX_APP_CMD_BUFFER_H

#include "../../stdlib/Types.h"
#include "Shader.h"
#include "ShaderUtils.h"
#include "../ShaderType.h"
#include "../../asset/Asset.h"
#include "../../command/AppCmdBuffer.h"
#include "GpuApiContainer.h"

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

void* cmd_shader_load(AppCmdBuffer*, Command*) {
    return NULL;
}

void* cmd_shader_load_sync(AppCmdBuffer* cb, Shader* shader, int32* shader_ids) {
    char asset_id[9];

    GpuApiContainer* gpu_api = (GpuApiContainer *) cb->gpu_api;

    ID3DBlob* shader_assets[SHADER_TYPE_SIZE];
    for (int32 i = 0; i < SHADER_TYPE_SIZE; ++i) {
        shader_assets[i] = NULL;
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
            shader_asset->ram_size
        );

        shader_asset->state |= ASSET_STATE_RAM_GC;
        shader_asset->state |= ASSET_STATE_VRAM_GC;
    }

    // Make shader/program
    shader->id = program_make(
        gpu_api->device, &gpu_api->pipeline, gpu_api->pipeline_layout,
        shader_assets[0], shader_assets[1], shader_assets[2]
    );

    return NULL;
}

#endif