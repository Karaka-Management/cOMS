/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_UI_UTILS_H
#define TOS_GPUAPI_UI_UTILS_H

#include <stdio.h>
#include <string.h>

void ui_input_create(Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    uint32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
)
{
    vertex_rect_border_create(
        vertices, index, zindex,
        x, y, width, height, 1, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
        12, 0.0f, 0.0f
    );

    vertex_rect_create(
        vertices, index, zindex,
        x + 1, y + 1, width - 2, height - 2, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
        14, 0.0f, 0.0f
    );
}

#endif