/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_OBJECT_HITBOX_H
#define TOS_OBJECT_HITBOX_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/FileUtils.cpp"
#else
    #include "../platform/linux/FileUtils.cpp"
#endif

struct Hitbox {

};

void hitbox_from_file_txt(
    RingMemory*,
    const char*,
    Hitbox*
)
{

}

void hitbox_from_file()
{

}

void hitbox_to_file()
{

}

#endif