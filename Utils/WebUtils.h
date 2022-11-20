/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_WEB_UTILS_H
#define UTILS_WEB_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "FileUtils.h"

namespace Utils {
    namespace WebUtils {
        int write_download_data (void *ptr, size_t size, size_t nmeb, void *stream)
        {
            Utils::FileUtils::file_body *out = (Utils::FileUtils::file_body *) stream;
            size_t outSize                   = size * nmeb;

            out->content = (char *) malloc(outSize + 1);
            if (!out->content) {
                fprintf(stderr, "CRITICAL: malloc failed");

                return 0;
            }

            if (out->content) {
                memcpy(out->content, ptr, outSize);

                out->size               = (int) outSize;
                out->content[out->size] = 0;
            }

            return out->size;
        }

        inline
        Utils::FileUtils::file_body download (char *url)
        {
            Utils::FileUtils::file_body page = {0};

            CURL *h = curl_easy_init();
            curl_easy_setopt(h, CURLOPT_URL, url);

            curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_download_data);
            curl_easy_setopt(h, CURLOPT_WRITEDATA, &page);
            curl_easy_perform(h);
            curl_easy_cleanup(h);

            return page;
        }
    }
}

#endif