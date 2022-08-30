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
    class FileUtils {
        private:

        public:
            static inline
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
    };
}

#endif