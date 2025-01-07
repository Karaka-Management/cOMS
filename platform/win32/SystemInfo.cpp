/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_SYSTEM_INFO_C
#define TOS_PLATFORM_WIN32_SYSTEM_INFO_C

#include <stdio.h>
#include <stdint.h>
#include "../../stdlib/Types.h"
#include "../../system/SystemInfo.h"

#include <psapi.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wbemidl.h>
#include <comdef.h>
#include <winnls.h>
#include <wingdi.h>
#include <hidsdi.h>

#if __aarch64__
    #include "../../stdlib/sve/SVE_Helper.h"
#else
    #include "../../stdlib/SIMD_Helper.h"
#endif

// @performance Do we really need all these libs, can't we simplify that?!
#include <intrin.h>
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "Ws2_32.lib")

// @todo implement for arm?

uint64 system_private_memory_usage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    HANDLE process = GetCurrentProcess();

    GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*) &pmc, sizeof(pmc));

    CloseHandle(process);

    return pmc.PrivateUsage;
}

uint64 system_app_memory_usage()
{
    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T address = 0;
    size_t total_size = 0;

    // MEM_IMAGE = DLL memory
    // MEM_MAPPED = Mapped files
    while (VirtualQueryEx(GetCurrentProcess(), (LPCVOID) address, &mbi, sizeof(mbi)) == sizeof(mbi)) {
        if (mbi.State == MEM_COMMIT && (mbi.Type == MEM_IMAGE || mbi.Type == MEM_MAPPED)) {
            total_size += mbi.RegionSize;
        }

        address += mbi.RegionSize;
    }

    return total_size;
}

uint16 system_language_code()
{
    LANGID lang_id = GetUserDefaultUILanguage();
    wchar_t local_name[LOCALE_NAME_MAX_LENGTH];

    if (!LCIDToLocaleName(lang_id, local_name, LOCALE_NAME_MAX_LENGTH, 0)) {
        return 0;
    }

    return (local_name[0] << 8) | local_name[1];
}

uint16 system_country_code()
{
    LANGID lang_id = GetUserDefaultUILanguage();
    wchar_t local_name[LOCALE_NAME_MAX_LENGTH];

    if (!LCIDToLocaleName(lang_id, local_name, LOCALE_NAME_MAX_LENGTH, 0)) {
        return 0;
    }

    return (local_name[3] << 8) | local_name[4];
}

void cache_info_get(int32 level, CpuCacheInfo* cache) {
    uint32 eax, ebx, ecx, edx;
    int32 type;

    cache->level = level;
    cache->size = 0;
    cache->ways = 0;
    cache->partitions = 0;
    cache->sets = 0;
    cache->line_size = 0;

    int32 regs[4];
    __cpuidex(regs, 4, level);
    eax = regs[0];
    ebx = regs[1];
    ecx = regs[2];
    edx = regs[3];

    type = (eax & 0x1F);

    if (type == 0) {
        return;
    }

    cache->ways = ((ebx >> 22) & 0x3FF) + 1;
    cache->line_size = (ebx & 0xFFF) + 1;
    cache->partitions = ((ebx >> 12) & 0x3FF) + 1;
    cache->sets = ecx + 1;
    cache->size = cache->ways * cache->partitions * cache->line_size * cache->sets;
}

void mainboard_info_get(MainboardInfo* info) {
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

int network_info_get(NetworkInfo* info) {
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

    int32 i = 0;

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

void cpu_info_get(CpuInfo* info) {
    info->simd.sse = max_sse_supported();
    info->simd.avx256 = max_avx256_supported();
    info->simd.avx512 = max_avx512_supported();
    info->simd.sve = max_sve_supported();
    info->simd.neon = max_neon_supported();
    info->simd.abm = supports_abm();

    cache_info_get(1, &info->cache[0]);
    cache_info_get(2, &info->cache[1]);
    cache_info_get(3, &info->cache[2]);
    cache_info_get(4, &info->cache[3]);

    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    info->thread_count = sys_info.dwNumberOfProcessors;
    info->page_size = sys_info.dwPageSize;

    int32 cpuInfo[4] = { 0 };
    __cpuid(cpuInfo, 0);

    memset(info->vendor, 0, sizeof(info->vendor));
    *((int32 *) info->vendor) = cpuInfo[1];
    *((int32 *) (info->vendor + 4)) = cpuInfo[3];
    *((int32 *) (info->vendor + 8)) = cpuInfo[2];
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

void os_info_get(OSInfo* info) {
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

        memcpy(info->vendor, "Microsoft", sizeof("Microsoft"));
        memcpy(info->name, "Windows", sizeof("Windows"));
        info->major = version_info.dwMajorVersion;
        info->minor = version_info.dwMinorVersion;
    #else
        memcpy(info->vendor, "Linux", sizeof("Linux"));
        memcpy(info->name, "Linux", sizeof("Linux"));
        info->major = 0;
        info->minor = 0;
    #endif
}

void ram_info_get(RamInfo* info) {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    info->memory = (int) (statex.ullTotalPhys / (1024 * 1024));
}

uint32 gpu_info_get(GpuInfo* info) {
    IDXGIFactory *pFactory = NULL;
    IDXGIAdapter *pAdapter = NULL;
    DXGI_ADAPTER_DESC adapterDesc;

    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
    if (FAILED(hr)) {
        return 0;
    }

    uint32 i = 0;
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

void display_info_get_primary(DisplayInfo* info) {
    DISPLAY_DEVICEA device;
    DEVMODEA mode;

    device.cb = sizeof(DISPLAY_DEVICEA);

    uint32_t i = 0;
    while (EnumDisplayDevicesA(NULL, i, &device, 0)) {
        ++i;

        if (!(device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)) {
            continue;
        }

        mode.dmSize = sizeof(mode);

        if (EnumDisplaySettingsA(device.DeviceName, ENUM_CURRENT_SETTINGS, &mode)) {
            str_copy_short(info->name, device.DeviceName);
            info->width = mode.dmPelsWidth;
            info->height = mode.dmPelsHeight;
            info->hz = mode.dmDisplayFrequency;
        }

        break;
    }
}

uint32 display_info_get(DisplayInfo* info) {
    DISPLAY_DEVICEA device;
    DEVMODEA mode;

    device.cb = sizeof(DISPLAY_DEVICEA);

    uint32 i = 0;

    while (EnumDisplayDevicesA(NULL, i, &device, 0)) {
        mode.dmSize = sizeof(mode);

        if (EnumDisplaySettingsA(device.DeviceName, ENUM_CURRENT_SETTINGS, &mode)) {
            str_copy_short(info[i].name, device.DeviceName);
            info[i].width = mode.dmPelsWidth;
            info[i].height = mode.dmPelsHeight;
            info[i].hz = mode.dmDisplayFrequency;
        }

        ++i;
    }

    return i;
}

void system_info_render(char* buf, const SystemInfo* info) {
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
        "Hardware\n" "Vendor: %s\n" "Brand: %s\n" "Model: %d\n" "Family: %d\n" "Mhz: %d\n" "Thread Count: %d\n" "Page Size: %d\n"
        "\n"
        "Cache:\n"
        "L1: Size %d Line %d\n"
        "L2: Size %d Line %d\n"
        "L3: Size %d Line %d\n"
        "L4: Size %d Line %d\n"
        "\n"
        "SIMD:\n" "SSE: %.1f\n" "AVX256: %d\n" "AVX512: %s\n" "SVE: %d\n" "NEON: %d\n" "ABM: %d\n"
        "\n"
        "GPU:\n"
        "==============\n"
        "Name: %s\n" "VRAM: %d\n"
        "Name: %s\n" "VRAM: %d\n"
        "\n"
        "Display:\n"
        "==============\n"
        "Name: %s\n" "Width: %d\n" "Height: %d\n" "Hz: %d\n"
        "Name: %s\n" "Width: %d\n" "Height: %d\n" "Hz: %d\n"
        "Name: %s\n" "Width: %d\n" "Height: %d\n" "Hz: %d\n"
        "Name: %s\n" "Width: %d\n" "Height: %d\n" "Hz: %d\n"
        "Name: %s\n" "Width: %d\n" "Height: %d\n" "Hz: %d\n"
        "Name: %s\n" "Width: %d\n" "Height: %d\n" "Hz: %d\n"
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
        info->cpu.vendor, info->cpu.brand, info->cpu.model, info->cpu.family, info->cpu.mhz, info->cpu.thread_count, info->cpu.page_size,
        info->cpu.cache[0].size, info->cpu.cache[0].line_size,
        info->cpu.cache[1].size, info->cpu.cache[1].line_size,
        info->cpu.cache[2].size, info->cpu.cache[2].line_size,
        info->cpu.cache[3].size, info->cpu.cache[3].line_size,
        info->cpu.simd.sse > 9 ? (f32) info->cpu.simd.sse / 10.0f : (f32) info->cpu.simd.sse, info->cpu.simd.avx256, info->cpu.simd.avx512 > 0 ? avx512[info->cpu.simd.avx512 - 1] : "0", info->cpu.simd.sve, info->cpu.simd.neon, (int32) info->cpu.simd.abm,
        info->gpu[0].name, info->gpu[0].vram,
        info->gpu_count < 2 ? "" : info->gpu[1].name, info->gpu_count < 2 ? 0 : info->gpu[1].vram,
        info->display[0].name, info->display[0].width, info->display[0].height, info->display[0].hz,
        info->display_count < 2 ? "" : info->display[1].name, info->display_count < 2 ? 0 : info->display[1].width, info->display_count < 2 ? 0 : info->display[1].height, info->display_count < 2 ? 0 : info->display[1].hz,
        info->display_count < 3 ? "" : info->display[2].name, info->display_count < 3 ? 0 : info->display[2].width, info->display_count < 3 ? 0 : info->display[2].height, info->display_count < 3 ? 0 : info->display[2].hz,
        info->display_count < 4 ? "" : info->display[3].name, info->display_count < 4 ? 0 : info->display[3].width, info->display_count < 4 ? 0 : info->display[3].height, info->display_count < 4 ? 0 : info->display[3].hz,
        info->display_count < 5 ? "" : info->display[4].name, info->display_count < 5 ? 0 : info->display[4].width, info->display_count < 5 ? 0 : info->display[4].height, info->display_count < 5 ? 0 : info->display[4].hz,
        info->display_count < 6 ? "" : info->display[5].name, info->display_count < 6 ? 0 : info->display[5].width, info->display_count < 6 ? 0 : info->display[5].height, info->display_count < 6 ? 0 : info->display[5].hz,
        info->ram.memory
    );
}

void system_info_get(SystemInfo* info)
{
    os_info_get(&info->os);
    mainboard_info_get(&info->mainboard);
    info->network_count = network_info_get(info->network);
    cpu_info_get(&info->cpu);
    ram_info_get(&info->ram);
    info->gpu_count = gpu_info_get(info->gpu);
    info->display_count = display_info_get(info->display);
    display_info_get_primary(&info->display_primary);
    info->language = system_language_code();
}

#endif