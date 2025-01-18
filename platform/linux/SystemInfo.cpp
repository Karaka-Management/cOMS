/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_SYSTEM_INFO_C
#define TOS_PLATFORM_LINUX_SYSTEM_INFO_C

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../stdlib/Types.h"
#include "../../system/SystemInfo.h"
#include "../../architecture/CpuInfo.cpp"

#include <locale.h>
#include <sys/resource.h>

// @todo Implement own line by line file reading

uint64 system_private_memory_usage()
{
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return usage.ru_maxrss * 1024;  // Convert from kilobytes to bytes
    } else {
        return 0;
    }
}

uint64 system_app_memory_usage()
{
    FILE* fp = fopen("/proc/self/smaps", "r");
    if (!fp) {
        return 0;
    }

    uint64 total_size = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (str_compare(line, "Private_Dirty:", sizeof("Private_Dirty:") - 1) == 0) {
            uint64 private_dirty;
            if (sscanf(line, "Private_Dirty: %lu kB", &private_dirty) == 1) {
                total_size += private_dirty * 1024;  // Convert from kB to bytes
            }
        }
    }

    fclose(fp);

    return total_size;
}

uint16 system_language_code()
{
    const char* localeName = setlocale(LC_ALL, "");

    return (localeName[0] << 8) | localeName[1];
}

uint16 system_country_code()
{
    const char* localeName = setlocale(LC_ALL, "");

    return (localeName[3] << 8) | localeName[4];
}

void mainboard_info_get(MainboardInfo* info) {
    info->name[63] = '\0';
    info->serial_number[63] = '\0';

    FILE *fp;

    fp = fopen("/sys/class/dmi/id/board_name", "r");
    if (fp) {
        fgets(info->name, 64, fp);
        fclose(fp);
    }

    fp = fopen("/sys/class/dmi/id/board_serial", "r");
    if (fp) {
        fgets(info->serial_number, 64, fp);
        fclose(fp);
    }

    info->name[strcspn(info->name, "\n")] = '\0';
    info->serial_number[strcspn(info->serial_number, "\n")] = '\0';
}

int32 network_info_get(NetworkInfo* info) {
    char path[256];
    struct stat st;
    int32 i = 0;

    for (i = 0; i < 4; i++) {
        sprintf_fast(path, "/sys/class/net/eth%d", i);

        if (stat(path, &st) == 0) {
            // Read MAC address
            sprintf_fast(path, "/sys/class/net/eth%d/address", i);
            FILE *mac_file = fopen(path, "r");
            if (mac_file) {
                fscanf(mac_file, "%s", info[i].mac);
                fclose(mac_file);
            }

            // Read interface name
            sprintf_fast(path, "/sys/class/net/eth%d/ifindex", i);
            FILE *index_file = fopen(path, "r");
            if (index_file) {
                fscanf(index_file, "%s", info[i].slot);
                fclose(index_file);
            }

            info[i].mac[23] = '\0';
            info[i].slot[63] = '\0';
        }
    }

    return i;
}

void cpu_info_get(CpuInfo* info) {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        if (str_compare(line, "vendor_id", 9) == 0) {
            sscanf(line, "vendor_id : %s", info->vendor);
        } else if (str_compare(line, "model", 5) == 0) {
            sscanf(line, "model : %d", &info->model);
        } else if (str_compare(line, "cpu MHz", 7) == 0) {
            sscanf(line, "cpu MHz : %d", &info->mhz);
        } else if (str_compare(line, "cpu cores", 10) == 0) {
            sscanf(line, "cpu cores : %d", &info->thread_coun);
        } else if (str_compare(line, "model name", 10) == 0) {
            sscanf(line, "model name : %63[^\n]", info->brand);
        }
    }

    fclose(fp);

    info->family = 0;
    info->page_size = 4096;  // Assuming standard page size of 4KB in Linux
}

void os_info_get(OSInfo* info) {
    memcpy(info->vendor, "Linux", sizeof("Linux"));
    memcpy(info->name, "Linux", sizeof("Linux"));
    info->major = 0;
    info->minor = 0;
}

void ram_info_get(RamInfo* info) {
    FILE* fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        return;
    }

    char line[256];
    uint32 total_memory = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %u kB", &total_memory) == 1) {
            break;
        }
    }

    fclose(fp);

    // Convert memory from kB to MB
    info->memory = total_memory / 1024;
}

uint32 gpu_info_get(GpuInfo* info) {
    FILE* fp = popen("lspci | grep VGA", "r");
    if (fp == NULL) {
        return 0;
    }

    uint32 count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        if (count >= 2) break;

        // Assuming that the first part of the line contains the name of the GPU
        char* gpu_name = strtok(line, ":");
        if (gpu_name) {
            sprintf_fast(info[count].name, sizeof(info[count].name), "%s", gpu_name);
        }

        // @todo this is Wrong
        info[count].vram = 2048;

        count++;
    }

    fclose(fp);

    return count;
}


void display_info_get_primary(DisplayInfo* info) {
    FILE* fp = popen("xrandr --current", "r");
    if (fp == NULL) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "primary")) {
            // Example of a line containing display info: "HDMI-1 connected 1920x1080+0+0 60.00*+"
            char name[64];
            uint32 width, height, hz;
            if (sscanf(line, "%s connected %dx%d+%*d+%*d %d", name, &width, &height, &hz) == 4) {
                str_copy_short(info->name, name);
                info->width = width;
                info->height = height;
                info->hz = hz;
            }
            break;
        }
    }

    fclose(fp);
}

uint32 display_info_get(DisplayInfo* info) {
    FILE* fp = popen("xrandr --current", "r");
    if (fp == NULL) {
        return 0;
    }

    char line[256];
    uint32 count = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "connected")) {
            // Example: "HDMI-1 connected 1920x1080+0+0 60.00*+"
            char name[64];
            uint32 width, height, hz;
            if (sscanf(line, "%s connected %dx%d+%*d+%*d %d", name, &width, &height, &hz) == 4) {
                str_copy_short(info[count].name, name);
                info[count].width = width;
                info[count].height = height;
                info[count].hz = hz;
                count++;
            }
        }
    }

    fclose(fp);

    return count;
}

#endif