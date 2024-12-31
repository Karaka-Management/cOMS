/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_OPENGL_GPU_API_CONTAINER
#define TOS_GPUAPI_OPENGL_GPU_API_CONTAINER

#include "../../stdlib/Types.h"
#include "OpenglUtils.h"

struct GpuApiContainer {
    uint32 frames_in_flight;
    uint32 framebuffer_idx;
    OpenglFrameData* framebuffers;
};

#endif