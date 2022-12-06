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
        static bool CURL_SETUP = false;

        inline
        void setup()
        {
            curl_global_init(CURL_GLOBAL_DEFAULT);
            Utils::WebUtils::CURL_SETUP = true;
        }

        inline
        void clean()
        {
            curl_global_cleanup();
            Utils::WebUtils::CURL_SETUP = false;
        }

        int write_download_data (void *ptr, size_t size, size_t nmeb, void *stream)
        {
            Utils::FileUtils::file_body *out = (Utils::FileUtils::file_body *) stream;
            size_t outSize                   = size * nmeb;

            if (out->size == 0) {
                // first time this function is called for a specific resource
                out->content = (char *) malloc((outSize + 1) * sizeof(char));
                if (!out->content) {
                    fprintf(stderr, "CRITICAL: malloc failed");

                    return 0;
                }

                if (out->content) {
                    memcpy(out->content, ptr, outSize * sizeof(char));

                    out->size               = (int) outSize;
                    out->content[out->size] = 0;
                }
            } else {
                // the max buffer (16384 = 16k) is exceeded, then this is called again and needs to get extended
                char *temp = (char *) malloc((outSize + out->size + 1) * sizeof(char));

                memcpy(temp, out->content, out->size * sizeof(char));
                memcpy(temp + out->size * sizeof(char), ptr, outSize * sizeof(char));

                free(out->content);

                out->content             = temp;
                out->size               += outSize;
                out->content[out->size]  = 0;
            }

            return out->size;
        }

        inline
        Utils::FileUtils::file_body download (char *url)
        {
            Utils::FileUtils::file_body page = {0};

            if (!Utils::WebUtils::CURL_SETUP) {
                Utils::WebUtils::setup();
            }

            CURL *h = curl_easy_init();
            curl_easy_setopt(h, CURLOPT_URL, url);
            curl_easy_setopt(h, CURLOPT_PRIVATE, url);
            curl_easy_setopt(h, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(h, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_download_data);
            curl_easy_setopt(h, CURLOPT_WRITEDATA, &page);
            curl_easy_perform(h);
            curl_easy_cleanup(h);

            return page;
        }

        inline
        void add_transfer(CURLM *cm, char *url, Utils::FileUtils::file_body *page, int *left)
        {
            CURL *h = curl_easy_init();
            curl_easy_setopt(h, CURLOPT_URL, url);
            curl_easy_setopt(h, CURLOPT_PRIVATE, url);
            curl_easy_setopt(h, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(h, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_download_data);
            curl_easy_setopt(h, CURLOPT_WRITEDATA, page);

            curl_multi_add_handle(cm, h);

            ++(*left);
        }

        Utils::FileUtils::file_body *multi_download (char **urls, int count, int max_parrallel)
        {
            Utils::FileUtils::file_body *pages = (Utils::FileUtils::file_body *) malloc(count * sizeof(Utils::FileUtils::file_body));

            if (!Utils::WebUtils::CURL_SETUP) {
                Utils::WebUtils::setup();
            }

            CURLM *cm = curl_multi_init();
            curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, max_parrallel);

            int downloads;
            int left = 0;

            for(downloads = 0; downloads < max_parrallel && downloads < count; ++downloads) {
                Utils::WebUtils::add_transfer(cm, urls[downloads], &pages[downloads], &left);
            }

            CURLMsg *msg;
            int msgs_left = -1;

            do {
                int alive = 1;
                curl_multi_perform(cm, &alive);

                while((msg = curl_multi_info_read(cm, &msgs_left))) {
                    if(msg->msg == CURLMSG_DONE) {
                        char *url;
                        CURL *e = msg->easy_handle;
                        curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
                        curl_multi_remove_handle(cm, e);
                        curl_easy_cleanup(e);
                        --left;
                    } else {
                        fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
                    }

                    if(downloads < count) {
                        ++downloads;
                        add_transfer(cm, urls[downloads], &pages[downloads], &left);
                    }
                }

                if(left > 0) {
                    curl_multi_wait(cm, NULL, 0, 1000, NULL);
                }

            } while(left > 0);

            curl_multi_cleanup(cm);

            return pages;
        }
    }
}

#endif