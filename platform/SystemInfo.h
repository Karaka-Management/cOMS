/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_SYSTEM_INFO_H
#define TOS_PLATFORM_SYSTEM_INFO_H

#include "../stdlib/Types.h"

struct CpuCacheInfo {
    int32 level;
    int32 size;
    int32 ways;
    int32 partitions;
    int32 sets;
    int32 line_size;
};

// @todo add vendor name
struct MainboardInfo {
    char name[64];
    char serial_number[64];
};

// @todo add ipv6
struct NetworkInfo {
    char slot[64];
    byte mac[8];
};

struct SIMDInfo {
    int32 sse;
    int32 avx256;
    int32 avx512;
    int32 sve;
    int32 neon;
    bool abm;
};

struct CpuInfo {
    char vendor[13];
    char brand[49];
    int32 model;
    int32 family;
    int32 thread_count;
    int32 mhz;
    CpuCacheInfo cache[4];
    int32 page_size;
    SIMDInfo simd;
};

struct OSInfo {
    char vendor[16];
    char name[64];
    int32 major;
    int32 minor;
};

struct RamInfo {
    int32 memory;
};

struct GpuInfo {
    char name[64];
    int32 vram;
};

struct DisplayInfo {
    char name[64];
    int32 width;
    int32 height;
    int32 hz;
};

struct SystemInfo {
    OSInfo os;
    MainboardInfo mainboard;

    NetworkInfo network[4];
    int32 network_count;

    CpuInfo cpu;
    RamInfo ram;

    GpuInfo gpu[2];
    int32 gpu_count;

    DisplayInfo display_primary;
    DisplayInfo display[6];
    int32 display_count;

    int32 language;
};

#endif