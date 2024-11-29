/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_OBJECT_ANIMATION_H
#define TOS_OBJECT_ANIMATION_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/FileUtils.cpp"
#else
    #include "../platform/linux/FileUtils.cpp"
#endif

struct Skeleton {

};

struct Animation {

};

void animation_from_file_txt(
    RingMemory*,
    const char*,
    Animation*
)
{

}

void animation_from_file()
{

}

void animation_to_file()
{

}

#endif