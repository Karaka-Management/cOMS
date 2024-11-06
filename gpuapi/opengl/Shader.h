/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_SHADER_H
#define TOS_GPUAPI_OPENGL_SHADER_H

#include "../../stdlib/Types.h"

struct Shader {
    uint32 shader_id;
    uint32 shader_locations[7];
    byte shader_data[16];
};

#endif