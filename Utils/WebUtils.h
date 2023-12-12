/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_WEB_UTILS_H
#define UTILS_WEB_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>

#include "FileUtils.h"

namespace Utils::WebUtils
{
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

    typedef struct {
        size_t size = 0;
        const char **resources = NULL;
    } ResourceTypes;

    typedef struct {
        size_t size = 0;
        const char **urls = NULL;
    } Urls;

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
    void add_transfer(CURLM *cm, char *url, int *left)
    {
        CURL *h = curl_easy_init();

        Utils::FileUtils::file_body *page = (Utils::FileUtils::file_body *) malloc(sizeof(Utils::FileUtils::file_body));
        page->size = 0;

        curl_easy_setopt(h, CURLOPT_URL, url);
        curl_easy_setopt(h, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(h, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_download_data);
        curl_easy_setopt(h, CURLOPT_WRITEDATA, page);
        curl_easy_setopt(h, CURLOPT_PRIVATE, page);

        curl_easy_setopt(h, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(h, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(h, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(h, CURLOPT_MAXREDIRS, 10L);
        curl_easy_setopt(h, CURLOPT_CONNECTTIMEOUT, 2L);
        curl_easy_setopt(h, CURLOPT_COOKIEFILE, "");
        curl_easy_setopt(h, CURLOPT_FILETIME, 1L);
        curl_easy_setopt(h, CURLOPT_USERAGENT, "firefox");
        curl_easy_setopt(h, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        curl_easy_setopt(h, CURLOPT_UNRESTRICTED_AUTH, 1L);
        curl_easy_setopt(h, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
        curl_easy_setopt(h, CURLOPT_EXPECT_100_TIMEOUT_MS, 0L);

        curl_multi_add_handle(cm, h);

        ++(*left);
    }

    size_t follow_links(CURLM *cm, Utils::FileUtils::file_body *page, char *url, int *left)
    {
        int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR |  HTML_PARSE_NOWARNING | HTML_PARSE_NONET;
        htmlDocPtr doc = htmlReadMemory(page->content, page->size, url, NULL, opts);

        if(!doc) {
            return 0;
        }

        xmlChar *xpath = (xmlChar*) "//img/@src";
        xmlXPathContextPtr context = xmlXPathNewContext(doc);
        xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
        xmlXPathFreeContext(context);

        if(!result) {
            return 0;
        }

        xmlNodeSetPtr nodeset = result->nodesetval;
        if(xmlXPathNodeSetIsEmpty(nodeset)) {
            xmlXPathFreeObject(result);
            return 0;
        }

        size_t count = 0;
        int i;

        for(i = 0; i < nodeset->nodeNr; i++) {
            double r = rand();
            int x = r * nodeset->nodeNr / RAND_MAX;
            const xmlNode *node = nodeset->nodeTab[x]->xmlChildrenNode;
            xmlChar *href = xmlNodeListGetString(doc, node, 1);

            // follow relative link
            if(true) {
                xmlChar *orig = href;

                // @todo consider base= tag which has an impact on relative links
                href = xmlBuildURI(href, (xmlChar *) url);
                xmlFree(orig);
            }

            char *link = (char *) href;
            if(!link || strlen(link) < 10) {
                continue;
            }

            if(!strncmp(link, "http://", 7) || !strncmp(link, "https://", 8) || !strncmp(link, "www.", 4)) {
                Utils::WebUtils::add_transfer(cm, link, left);

                // limit to max 1000 links per page to follow
                if(count++ == 1000) {
                    break;
                }
            }

            xmlFree(link);
        }

        xmlXPathFreeObject(result);

        return count;
    }

    void *multi_download(
        Urls urls,
        const char *baseDir,
        int max_parrallel = 1,
        ResourceTypes *resources = NULL
    ) {
        if (!Utils::WebUtils::CURL_SETUP) {
            Utils::WebUtils::setup();
        }

        CURLM *cm = curl_multi_init();
        curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, max_parrallel);
        curl_multi_setopt(cm, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_parrallel);
        curl_multi_setopt(cm, CURLMOPT_MAX_HOST_CONNECTIONS, 5);

        #ifdef CURLPIPE_MULTIPLEX
            curl_multi_setopt(cm, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
        #endif

        int downloads = 0;
        int left = 0;

        for(downloads = 0; downloads < max_parrallel && downloads < urls.size; ++downloads) {
            Utils::WebUtils::add_transfer(cm, urls.urls[downloads], &left);
        }

        CURLMsg *msg;
        int msgs_left = -1;

        do {
            int alive = 1;
            curl_multi_perform(cm, &alive);

            while((msg = curl_multi_info_read(cm, &msgs_left))) {
                if(msg->msg == CURLMSG_DONE) {
                    CURL *e = msg->easy_handle;
                    char *url;
                    Utils::FileUtils::file_body *page;

                    curl_easy_getinfo(e, CURLINFO_PRIVATE, &page);
                    curl_easy_getinfo(e, CURLINFO_EFFECTIVE_URL, &url);

                    if (msg->data.result == CURLE_OK) {
                        long statusCode = 0;
                        curl_easy_getinfo(e, CURLINFO_RESPONSE_CDE, &statusCode);

                        if (statusCode == 200) {
                            char *ctype;
                            curl_easy_getinfo(e, CURLINFO_CONTENT_TYPE, &ctype);

                            // @todo save file (how to handle base resources, either pass base url or save based on url?)

                            if (ctype != NULL && strlen(ctype) > 10 && strstr(ctype, "text/html")) {
                                // @todo check limits
                                left += follow_links(cm, page, url, &left);
                            }
                        }
                    }

                    curl_multi_remove_handle(cm, e);
                    curl_easy_cleanup(e);
                    free(page->content);
                    free(page);

                    --left;
                } else {
                    fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
                }

                if(downloads < urls.size) {
                    ++downloads;
                    Utils::WebUtils::add_transfer(cm, urls.urls[downloads], &left);
                }
            }

            if(left > 0) {
                curl_multi_wait(cm, NULL, 0, 1000, NULL);
            }
        } while(left > 0);

        curl_multi_cleanup(cm);

        return null;
    }
}

#endif