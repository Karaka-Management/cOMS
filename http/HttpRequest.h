/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_JINGGA_HTTP_REQUEST_H
#define COMS_JINGGA_HTTP_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stdlib/Types.h"
#include "../utils/StringUtils.h"
#include "../log/Log.h"

#include "HttpMethod.h"
#include "HttpProtocol.h"
#include "HttpHeaderKey.h"
#include "HttpUri.h"
#include "HttpHeader.h"

/**
 * Data layout
 *  HttpRequest
 *      ...
 *  Request body (excl. file data if available)
 *  HttpHeader[]
 *
 */

// The order of the members and their types is carefully chosen to make them fit into 1 cache line
// The request content must come directly after the request
struct HttpRequest {
    HttpUri uri;
    HttpMethod method;
    HttpProtocol protocol;

    uint16 header_count;
    uint16 body_offset;
    uint32 body_length;
    uint32 request_length;
    uint32 headers_offset;
};

inline
bool http_request_header_is_complete(const char* request, size_t length) {
    return str_contains(request, "\r\n\r\n", length);
}

// Binary search for the key
inline
const HttpHeader* http_request_header_get(const HttpRequest* request, HttpHeaderKey key) {
    const HttpHeader* base = (HttpHeader *) ((uintptr_t) request + sizeof(HttpRequest) + request->request_length);
    int32 header_count = OMS_MIN(request->header_count, (uint16) key);

    while (header_count > 1) {
        int32 half = header_count / 2;
        header_count -= half;

        base += (base[half - 1].key < key) * half;
    }

    return base->key == key ? base : NULL;
}

inline
const char* http_request_header_value_get(const HttpRequest* request, const HttpHeader* header) {
    const char* request_data = (const char *) ((uintptr_t) request + sizeof(HttpRequest));

    return request_data + header->value_offset;
}

bool http_request_has_file_upload(const HttpRequest* request) {
    const HttpHeader* header = http_request_header_get(request, HTTP_HEADER_KEY_CONTENT_TYPE);
    if (!header) {
        return false;
    }

    const char* header_value = http_request_header_value_get(request, header);
    if ((str_compare_caseless(header_value, "application/", OMS_MIN(header->value_length, sizeof("application/") - 1)) == 0
            && str_compare_caseless(header_value, "application/json", OMS_MIN(header->value_length, sizeof("application/json") - 1)) != 0)
        || str_compare_caseless(header_value, "image/", OMS_MIN(header->value_length, sizeof("image/") - 1)) == 0
        || str_compare_caseless(header_value, "audio/", OMS_MIN(header->value_length, sizeof("audio/") - 1)) == 0
        || str_compare_caseless(header_value, "video/", OMS_MIN(header->value_length, sizeof("video/") - 1)) == 0
        || str_compare_caseless(header_value, "text/csv", OMS_MIN(header->value_length, sizeof("text/csv") - 1)) == 0
    ) {
        return true;
    }

    if (str_compare_caseless(header_value, "multipart/form-data", OMS_MIN(header->value_length, sizeof("multipart/form-data") - 1)) != 0) {
        return false;
    }

    // @todo not every form-data is a file upload but it is at least possible

    return false;
}

void http_request_header_parse(HttpRequest* http_request, const char* request) {
    const char* request_start = request;

    //////////////////////////////////////////////////
    // Parsing HTTP request line
    //////////////////////////////////////////////////

    str_skip_empty(&request);

    // Parse request type
    if (str_compare(request, "GET") == 0) {
        http_request->method = HTTP_METHOD_GET;
    } else if (str_compare(request, "POST") == 0) {
        http_request->method = HTTP_METHOD_POST;
    } else if (str_compare(request, "PUT") == 0) {
        http_request->method = HTTP_METHOD_PUT;
    } else if (str_compare(request, "DELETE") == 0) {
        http_request->method = HTTP_METHOD_DELETE;
    } else {
        // Additional request types are possible BUT we don't support them in our internal framework
        // If this would be a public framework we would've to support additional request types
        http_request->method = HTTP_METHOD_UNKNOWN;
    }

    // Parse reuqest path
    str_move_past(&request, ' ');
    http_request->uri.path_offset = request - request_start;

    str_skip_until_list(&request, ":?# ");
    http_request->uri.path_length = (request - request_start) - http_request->uri.path_offset;

    // Parse port
    if (*request == ':') {
        http_request->uri.port = (uint16) str_to_int(request, &request);
    }

    // Parse query parameters
    if (*request == '?') {
        http_request->uri.parameter_offset = request - request_start;
        str_skip_until_list(&request, "# ");
        http_request->uri.path_length = (request - request_start) - http_request->uri.parameter_offset;
    }

    // Parse fragment
    if (*request == '#') {
        http_request->uri.fragment_offset = request - request_start;
        str_move_to(&request, ' ');
        http_request->uri.fragment_length = (request - request_start) - http_request->uri.fragment_offset;
    }

    // Parse protocol
    str_move_past(&request, ' ');
    if (str_compare(request, "HTTP/", sizeof("HTTP/") - 1) != 0) {
        LOG_1("Invalid HTTP header, no protocol defined");
        ASSERT_SIMPLE(false);

        return;
    }

    request += sizeof("HTTP/") - 1;
    if (*request == '1') {
        http_request->protocol = HTTP_PROTOCOL_1_1;
    } else if (*request == '2') {
        http_request->protocol = HTTP_PROTOCOL_2;
    } else if (*request == '3') {
        http_request->protocol = HTTP_PROTOCOL_3;
    } else {
        http_request->protocol = HTTP_PROTOCOL_UNKNOWN;
    }

    //////////////////////////////////////////////////
    // Parsing HTTP headers
    //////////////////////////////////////////////////
    // The HTTP headers end with \r\n\r\n (= one empty line/element)
    while (request[0] != '\r' && request[1] != '\n' && request[2] != '\r' && request[3] != '\n') {
        str_move_past(&request, '\n');

        // @todo parse headers
    }

    //////////////////////////////////////////////////
    // Parsing HTTP body
    //////////////////////////////////////////////////
    request += 4;
    http_request->body_offset = request - request_start;
    http_request->body_length = http_request->request_length - http_request->body_offset;
}

void parse_multipart_data(const char *body, const char *boundary) {
    char *buffer = strdup(body);
    if (!buffer) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }

    char *part = strtok(buffer, boundary);
    while (part) {
        // Skip leading and trailing newlines
        while (*part == '\r' || *part == '\n') part++;

        // Parse part headers and content
        char* headers_end = strstr(part, "\r\n\r\n");
        if (headers_end) {
            *headers_end = '\0'; // Terminate headers
            char *content = headers_end + 4; // Skip "\r\n\r\n"

            printf("Part Headers:\n%s\n", part);
            printf("Part Content:\n%s\n", content);
        }

        part = strtok(NULL, boundary);
    }

    free(buffer);
}

/*
int main() {
    const char *http_request =
        "POST /submit HTTP/1.1\n"
        "Host: www.example.com\n"
        "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\n"
        "Content-Length: 348\n"
        "\n"
        "------WebKitFormBoundary7MA4YWxkTrZu0gW\n"
        "Content-Disposition: form-data; name=\"username\"\n"
        "\n"
        "john_doe\n"
        "------WebKitFormBoundary7MA4YWxkTrZu0gW\n"
        "Content-Disposition: form-data; name=\"file\"; filename=\"example.txt\"\n"
        "Content-Type: text/plain\n"
        "\n"
        "This is a sample file.\n"
        "------WebKitFormBoundary7MA4YWxkTrZu0gW--\n";

    HttpRequest request;
    http_request_parse(http_request, &request);
    print_http_request(&request);

    // Parse multipart data if Content-Type is multipart/form-data
    for (int i = 0; i < request.header_count; i++) {
        if (strstr(request.headers[i], "Content-Type: multipart/form-data")) {
            const char *boundary_start = strstr(request.headers[i], "boundary=");
            if (boundary_start) {
                char boundary[128];
                sscanf(boundary_start, "boundary=%127s", boundary);
                printf("\nParsing multipart data with boundary: %s\n", boundary);
                parse_multipart_data(request.body, boundary);
            }
            break;
        }
    }

    return 0;
}
    */

#endif