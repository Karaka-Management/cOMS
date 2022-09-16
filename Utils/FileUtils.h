/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_FILE_UTILS_H
#define UTILS_FILE_UTILS_H

#ifdef _WIN32
    #include <windows.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wchar.h>

    #ifdef _MSC_VER
        #include <io.h>
    #else
        #include <unistd.h>
    #endif

    #include <stdbool.h>
#elif defined __linux__
    #include <sys/stat.h>
#endif

namespace Utils {
    namespace FileUtils {
        inline
        bool file_exists (char *filename)
        {
            #ifdef _WIN32
                #ifdef _MSC_VER
                    return _access_s(filename, 0) == 0;
                #else
                    return access(filename, 0) == 0;
                #endif
            #elif defined __linux__
                struct stat buffer;
                return stat(filename, &buffer) == 0;
            #endif
        }

        inline
        time_t last_modification (char *filename)
        {
            #ifdef _WIN32
                FILETIME modtime;
                HANDLE h;

                h = CreateFileW(filename, GENERIC_READ | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);
                if (h == INVALID_HANDLE_VALUE) {
                    return (time_t) 0;
                }

                if (GetFileTime(h, NULL, NULL, &modtime) == 0) {
                    return (time_t) 0;
                }

                unsinged long long seconds = modtime.dwHighDateTime << 32;
                seconds |= modtime.dwLowDateTime;

                return (seconds - 116444736000000000) / 10000000;
            #elif defined __linux__
                struct stat buffer;
                stat(filename, &buffer);

                (time_t) buffer.st_mtim.tv_sec;
            #endif
        }

        char* file_extension (char *filename)
        {
            char *dot = strrchr(filename, '.');

            if (!dot || dot == filename) {
                return "";
            }

            return dot + 1;
        }

        typedef struct {
            char *content;
            int size;
        } file_body;

        inline
        file_body read_file (char *filename)
        {
            file_body file = {0};

            FILE *fp = fopen(filename, "rb");
            if (!fp) {
                return NULL;
            }

            fseek(fp, 0, SEEK_END);
            file.size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            file.content = malloc(file.size);
            if (!file.content) {
                fprintf(stderr, "CRITICAL: malloc failed");
            }

            fread(file.content, file.size, 1, fp);

            fclose(fp);

            return file;
        }
}

#endif