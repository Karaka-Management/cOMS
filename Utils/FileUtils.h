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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <wchar.h>
#else
    #include <sys/stat.h>
#endif

#include "OSWrapper.h"

namespace Utils
{
    namespace FileUtils
    {
        inline
        bool file_exists (const char *filename)
        {
            #ifdef _WIN32
                return access(filename, 0) == 0;
            #else
                struct stat buffer;
                return stat(filename, &buffer) == 0;
            #endif
        }

        inline
        time_t last_modification (const char *filename)
        {
            #ifdef _WIN32
                FILETIME modtime;
                HANDLE h;

                size_t nameLength = strlen(filename);

                wchar_t *wtext = (wchar_t *) calloc(nameLength, sizeof(char));
                mbstowcs_s(NULL, wtext, nameLength, filename, nameLength);
                LPWSTR pFilename = wtext;

                if (!pFilename) {
                    free(wtext);

                    return 0;
                }

                h = CreateFileW(pFilename, GENERIC_READ | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL);

                free(wtext);
                free(pFilename);

                if (h == INVALID_HANDLE_VALUE) {
                    return (time_t) 0;
                }

                if (GetFileTime(h, NULL, NULL, &modtime) == 0) {
                    return (time_t) 0;
                }

                unsigned long long seconds = ((unsigned long long) (modtime.dwHighDateTime)) << 32;
                seconds |= modtime.dwLowDateTime;

                return (seconds - 116444736000000000) / 10000000;
            #else
                struct stat buffer;
                stat(filename, &buffer);

                return (time_t) buffer.st_mtim.tv_sec;
            #endif

            return (time_t) 0;
        }

        inline
        const char* file_extension (const char *filename)
        {
            char *dot = strrchr((char *) filename, '.');

            if (!dot || dot == filename) {
                return "";
            }

            return dot + 1;
        }

        typedef struct {
            char *content;
            int size = 0; // doesn't include null termination (same as strlen)
        } file_body;

        file_body read_file (const char *filename)
        {
            file_body file = {0};

            FILE *fp = fopen(filename, "rb");
            if (!fp) {
                return file;
            }

            fseek(fp, 0, SEEK_END);
            file.size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            file.content = (char *) malloc((file.size + 1) * sizeof(char));
            if (!file.content) {
                fprintf(stderr, "CRITICAL: malloc failed");

                return file;
            }

            fread(file.content, file.size, 1, fp);
            file.content[file.size] = 0;

            fclose(fp);

            return file;
        }
    }
}

#endif
