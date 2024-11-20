/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREAD_DEFINES_H
#define TOS_PLATFORM_LINUX_THREAD_DEFINES_H

#include <pthread.h>
#include <unistd.h>
typedef void* (*ThreadJobFunc)(void*);

#define THREAD_RETURN void*

#endif