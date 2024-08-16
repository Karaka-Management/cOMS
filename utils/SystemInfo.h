/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_SYSTEM_INFO_H
#define TOS_UTILS_SYSTEM_INFO_H

#include <stdio.h>
#include <stdint.h>
#include "StringUtils.h"
#include "../stdlib/Types.h"
#include "../stdlib/simd/SIMD_Helper.h"

#if _WIN32
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <d3d11.h>
    #include <dxgi.h>
    #include <wbemidl.h>
    #include <comdef.h>
#endif

#ifdef _MSC_VER
    #include <intrin.h>
#endif

#if __linux__ && (__i386__ || __x86_64__)
    #include <cpuid.h>
#endif

// @todo implement for arm?
// @todo implement for linux?
// @todo move to platform specifc files

struct CpuCacheInfo {
    int level;
    int size;
    int ways;
    int partitions;
    int sets;
    int line_size;
};

void get_cache_info(int level, CpuCacheInfo* cache) {
    unsigned int eax, ebx, ecx, edx;
    int type;

    cache->level = level;
    cache->size = 0;
    cache->ways = 0;
    cache->partitions = 0;
    cache->sets = 0;
    cache->line_size = 0;

    #if _WIN32
        int regs[4];
        __cpuidex(regs, 4, level);
        eax = regs[0];
        ebx = regs[1];
        ecx = regs[2];
        edx = regs[3];

        type = (eax & 0x1F);
    #else
        __cpuid_count(4, level, eax, ebx, ecx, edx);

        type = (eax & 0x1F);
    #endif

    if (type == 0) {
        return;
    }

    cache->ways = ((ebx >> 22) & 0x3FF) + 1;
    cache->line_size = (ebx & 0xFFF) + 1;
    cache->partitions = ((ebx >> 12) & 0x3FF) + 1;
    cache->sets = ecx + 1;
    cache->size = cache->ways * cache->partitions * cache->line_size * cache->sets;
}

// @todo add vendor name
struct MainboardInfo {
    char name[64];
    char serial_number[64];
};

void get_mainboard_info(MainboardInfo* info) {
    info->name[63] = '\0';
    info->serial_number[63] = '\0';

    HRESULT hres;

    // Step 1: Initialize COM library
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        return;
    }

    // Step 2: Set general COM security levels
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    if (FAILED(hres)) {
        CoUninitialize();
        return;
    }

    // Step 3: Obtain initial locator to WMI
    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID *)&pLoc
    );
    if (FAILED(hres)) {
        CoUninitialize();
        return;
    }

    // Step 4: Connect to WMI through IWbemLocator::ConnectServer
    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );
    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // Step 5: Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // Step 6: Use the IWbemServices pointer to make a WMI query
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_BaseBoard"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    // Step 7: Retrieve the data
    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        hr = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            snprintf(info->name, 64, "%S", vtProp.bstrVal);
            VariantClear(&vtProp);
        }

        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            snprintf(info->serial_number, 63, "%S", vtProp.bstrVal);
            info->serial_number[64] = '\0';

            VariantClear(&vtProp);
        }

        pclsObj->Release();
    }

    // Clean up
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// @todo add ipv6
struct NetworkInfo {
    char slot[64];
    byte mac[8];
};

int get_network_info(NetworkInfo* info) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 0;
    }

    DWORD dwSize = 0;
    PIP_ADAPTER_ADDRESSES pAdapterAddresses = NULL;
    PIP_ADAPTER_ADDRESSES pAdapter = NULL;

    // Get the size of the adapter addresses buffer
    if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL, &dwSize) == ERROR_BUFFER_OVERFLOW) {
        pAdapterAddresses = (PIP_ADAPTER_ADDRESSES) malloc(dwSize);
        if (pAdapterAddresses == NULL) {
            WSACleanup();
            return 0;
        }
    } else {
        WSACleanup();
        return 0;
    }

    // Get the adapter addresses
    if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAdapterAddresses, &dwSize) != NO_ERROR) {
        free(pAdapterAddresses);
        WSACleanup();
        return 0;
    }

    int i = 0;

    // Iterate over the adapters and print their MAC addresses
    pAdapter = pAdapterAddresses;
    while (pAdapter && i < 4) {
        if (pAdapter->PhysicalAddressLength != 0) {
            info[i].slot[63] = '\0';
            info[i].mac[23] = '\0';

            memcpy(info[i].mac, pAdapter->PhysicalAddress, 8);
            wcstombs(info[i].slot, pAdapter->FriendlyName, 63);

            ++i;
        }

        pAdapter = pAdapter->Next;
    }

    free(pAdapterAddresses);
    WSACleanup();

    return i;
}

struct SIMDInfo {
    float sse;
    int avx256;
    int avx512;
};

struct CpuInfo {
    char vendor[13];
    char brand[49];
    int model;
    int family;
    int mhz;
    CpuCacheInfo cache[4];
    int page_size;
    SIMDInfo simd;
};

void get_cpu_info(CpuInfo* info) {
    int temp;
    info->simd.sse = (temp = max_sse_supported()) > 9 ? temp / 10.0f : temp;
    info->simd.avx256 = max_avx256_supported();
    info->simd.avx512 = max_avx512_supported();

    get_cache_info(1, &info->cache[0]);
    get_cache_info(2, &info->cache[1]);
    get_cache_info(3, &info->cache[2]);
    get_cache_info(4, &info->cache[3]);

    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    info->page_size = sys_info.dwPageSize;

    int cpuInfo[4] = { 0 };
    __cpuid(cpuInfo, 0);

    memset(info->vendor, 0, sizeof(info->vendor));
    *((int*)info->vendor) = cpuInfo[1];
    *((int*)(info->vendor + 4)) = cpuInfo[3];
    *((int*)(info->vendor + 8)) = cpuInfo[2];
    info->vendor[12] = '\0';

    __cpuid(cpuInfo, 0x80000002);
    memcpy(info->brand, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000003);
    memcpy(info->brand + 16, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000004);
    memcpy(info->brand + 32, cpuInfo, sizeof(cpuInfo));
    info->brand[48] = '\0';

    __cpuid(cpuInfo, 1);
    info->model = (cpuInfo[0] >> 4) & 0xF;
    info->family = (cpuInfo[0] >> 8) & 0xF;

    DWORD bufSize = sizeof(DWORD);
    HKEY hKey;
    long lError = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKey
    );

    if (lError == ERROR_SUCCESS) {
        RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE) &(info->mhz), &bufSize);
    }

    RegCloseKey(hKey);
}

struct OSInfo {
    char vendor[16];
    char name[64];
    int major;
    int minor;
};

void get_os_info(OSInfo* info) {
    info->vendor[15] = '\0';
    info->name[63] = '\0';

    #if defined(_WIN32) || defined(_WIN64)
        OSVERSIONINFOEXW version_info;
        memset(&version_info, 0, sizeof(OSVERSIONINFOEXW));
        version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
        NTSTATUS(WINAPI *RtlGetVersion)(OSVERSIONINFOEXW*) = (NTSTATUS(WINAPI *)(OSVERSIONINFOEXW*))GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");
        if (RtlGetVersion != nullptr) {
            RtlGetVersion(&version_info);
        }

        strcpy(info->vendor, "Microsoft");
        strcpy(info->name, "Windows");
        info->major = version_info.dwMajorVersion;
        info->minor = version_info.dwMinorVersion;
    #else
        strcpy(info->vendor, "Linux");
        strcpy(info->name, "Linux");
        info->major = 0;
        info->minor = 0;
    #endif
}

struct RamInfo {
    int memory;
};

void get_ram_info(RamInfo* info) {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    info->memory = (int) (statex.ullTotalPhys / (1024 * 1024));
}

struct GpuInfo {
    char name[64];
    int vram;
};

unsigned int get_gpu_info(GpuInfo* info) {
    IDXGIFactory *pFactory = NULL;
    IDXGIAdapter *pAdapter = NULL;
    DXGI_ADAPTER_DESC adapterDesc;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
    if (FAILED(hr)) {
        return 0;
    }

    UINT i = 0;
    while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND && i < 2) {
        hr = pAdapter->GetDesc(&adapterDesc);
        if (FAILED(hr)) {
            pAdapter->Release();
            break;
        }

        wcstombs(info[i].name, adapterDesc.Description, 63);
        info[i].name[63] = '\0';
        info[i].vram = (int) (adapterDesc.DedicatedVideoMemory / (1024 * 1024));

        pAdapter->Release();
        i++;
    }

    pFactory->Release();

    return i;
}

struct SystemInfo {
    OSInfo os;
    MainboardInfo mainboard;

    NetworkInfo network[4];
    int network_count;

    CpuInfo cpu;
    RamInfo ram;

    GpuInfo gpu[2];
    int gpu_count;
};

void render_system_info(char* buf, const SystemInfo* info) {
    const char avx512[8][12] = {
        "AVX-512F",
        "AVX-512DQ",
        "AVX-512IFMA",
        "AVX-512PF",
        "AVX-512ER",
        "AVX-512CD",
        "AVX-512BW",
        "AVX-512VL"
    };

    sprintf_s(
        buf,
        4096,
        "OS:\n"
        "==============\n"
        "Vendor: %s\n" "Name: %s\n" "Major: %d\n" "Minor: %d\n"
        "\n"
        "Mainboard:\n"
        "==============\n"
        "Name: %s\n" "SN: %s\n"
        "\n"
        "Network:\n"
        "==============\n"
        "Slot: %s\n" "MAC: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n"
        "\n"
        "Slot: %s\n" "MAC: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n"
        "\n"
        "Slot: %s\n" "MAC: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n"
        "\n"
        "Slot: %s\n" "MAC: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X\n"
        "\n"
        "CPU:\n"
        "==============\n"
        "Hardware\n" "Vendor: %s\n" "Brand: %s\n" "Model: %d\n" "Family: %d\n" "Mhz: %d\n" "Page Size: %d\n"
        "\n"
        "Cache:\n"
        "L1: Size %d Line %d\n"
        "L2: Size %d Line %d\n"
        "L3: Size %d Line %d\n"
        "L4: Size %d Line %d\n"
        "\n"
        "SIMD:\n" "SSE: %.1f\n" "AVX256: %d\n" "AVX512: %s\n"
        "\n"
        "GPU:\n"
        "==============\n"
        "Name: %s\n" "VRAM: %d\n"
        "\n"
        "Name: %s\n" "VRAM: %d\n"
        "\n"
        "RAM:\n"
        "==============\n"
        "Memory: %d MB",
        info->os.vendor, info->os.name, info->os.major, info->os.minor,
        info->mainboard.name, info->mainboard.serial_number,
        info->network[0].slot, info->network[0].mac[0], info->network[0].mac[1], info->network[0].mac[2], info->network[0].mac[3], info->network[0].mac[4], info->network[0].mac[5], info->network[0].mac[6], info->network[0].mac[7],
        info->network_count < 2 ? "" : info->network[1].slot, info->network_count < 2 ? 0 : info->network[1].mac[0], info->network_count < 2 ? 0 : info->network[1].mac[1], info->network_count < 2 ? 0 : info->network[1].mac[2], info->network_count < 2 ? 0 : info->network[1].mac[3], info->network_count < 2 ? 0 : info->network[1].mac[4], info->network_count < 2 ? 0 : info->network[1].mac[5], info->network_count < 2 ? 0 : info->network[1].mac[6], info->network_count < 2 ? 0 : info->network[1].mac[7],
        info->network_count < 3 ? "" : info->network[2].slot, info->network_count < 3 ? 0 : info->network[2].mac[0], info->network_count < 3 ? 0 : info->network[2].mac[1], info->network_count < 3 ? 0 : info->network[2].mac[2], info->network_count < 3 ? 0 : info->network[2].mac[3], info->network_count < 3 ? 0 : info->network[2].mac[4], info->network_count < 3 ? 0 : info->network[2].mac[5], info->network_count < 3 ? 0 : info->network[2].mac[6], info->network_count < 3 ? 0 : info->network[2].mac[7],
        info->network_count < 4 ? "" : info->network[3].slot, info->network_count < 4 ? 0 : info->network[3].mac[0], info->network_count < 4 ? 0 : info->network[3].mac[1], info->network_count < 4 ? 0 : info->network[3].mac[2], info->network_count < 4 ? 0 : info->network[3].mac[3], info->network_count < 4 ? 0 : info->network[3].mac[4], info->network_count < 4 ? 0 : info->network[3].mac[5], info->network_count < 4 ? 0 : info->network[3].mac[6], info->network_count < 4 ? 0 : info->network[3].mac[7],
        info->cpu.vendor, info->cpu.brand, info->cpu.model, info->cpu.family, info->cpu.mhz, info->cpu.page_size,
        info->cpu.cache[0].size, info->cpu.cache[0].line_size,
        info->cpu.cache[1].size, info->cpu.cache[1].line_size,
        info->cpu.cache[2].size, info->cpu.cache[2].line_size,
        info->cpu.cache[3].size, info->cpu.cache[3].line_size,
        info->cpu.simd.sse, info->cpu.simd.avx256, info->cpu.simd.avx512 > 0 ? avx512[info->cpu.simd.avx512 - 1] : "0",
        info->gpu[0].name, info->gpu[0].vram,
        info->gpu_count < 2 ? "" : info->gpu[1].name, info->gpu_count < 2 ? 0 : info->gpu[1].vram,
        info->ram.memory
    );
}

void get_system_info(SystemInfo* info)
{
    get_os_info(&info->os);
    get_mainboard_info(&info->mainboard);
    info->network_count = get_network_info(info->network);
    get_cpu_info(&info->cpu);
    get_ram_info(&info->ram);
    info->gpu_count = get_gpu_info(info->gpu);
}

#endif