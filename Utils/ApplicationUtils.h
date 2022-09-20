/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_APPLICATION_UTILS_H
#define UTILS_APPLICATION_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>
#elif defined __linux__
    #include <unistd.h>
#endif

namespace Utils {
    namespace ApplicationUtils {
        inline
        char *cwd()
        {
            char *cwd = (char *) malloc(sizeof(char) * 4096);
            if (cwd == NULL) {
                return NULL;
            }

            #ifdef _WIN32
                _getcwd(cwd, sizeof(char));
            #else
                getcwd(cwd, sizeof(char));
            #endif

            return cwd;
        }

        void chdir_application(char *cwd, char *arg)
        {
            #ifdef _WIN32
                char *dir;

                char *pos = strrchr(arg, '/');
                if (pos == NULL) {
                    pos = strrchr(arg, '\\');
                }

                if (pos != NULL) {
                    memcpy(dir, cwd, (cwd - pos + 1) * sizeof(char));

                    _chdir(dir);

                    free(dir);
                    free(pos);
            #else
                char *dir;

                char *pos = strrchr(arg, '/');
                if (pos == NULL) {
                    pos = strrchr(arg, '\\');
                }

                if (pos != NULL) {
                    memcpy(dir, cwd, (cwd - pos + 1) * sizeof(char));

                    chdir(dir);

                    free(dir);
                    free(pos);
                }
            #endif
        }

        char *compile_arg_line(int argc, char **argv)
        {
            size_t max    = 512;
            size_t length = 0;
            char *arg     = (char *) calloc(max, sizeof(char));

            for (int i = 1; i < argc; ++i) {
                size_t argv_length = strlen(argv[i]);
                if (length + strlen(argv[i]) + 1 > max) {
                    char *tmp = (char *) calloc(max + 128, sizeof(char));
                    memcpy(tmp, arg, (length + 1) * sizeof(char));

                    free(arg);
                    arg  = tmp;
                    max += 128;
                }

                strcat(arg, argv[i]);
                length += argv_length;
            }

            return arg;
        }
    }
}

#endif