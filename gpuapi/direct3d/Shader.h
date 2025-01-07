/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECT3D_SHADER_H
#define TOS_GPUAPI_DIRECT3D_SHADER_H

#include "../../stdlib/Types.h"

struct Shader {
    uint32 id;
    uint32 locations[7];
    byte data[16];
};

#endif