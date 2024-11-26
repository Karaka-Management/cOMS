/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREADING_THREAD_H
#define TOS_PLATFORM_LINUX_THREADING_THREAD_H

#include <unistd.h>
#include "../../../stdlib/Types.h"
#include "ThreadDefines.h"

uint32 pcthread_get_num_procs()
{
    return (uint32) sysconf(_SC_NPROCESSORS_ONLN);
}

#endif