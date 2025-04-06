/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_JINGGA_HTTP_RESPONSE_H
#define COMS_JINGGA_HTTP_RESPONSE_H

#include "../stdlib/Types.h"

#include "HttpMethod.h"
#include "HttpProtocol.h"
#include "HttpStatusCode.h"

struct HttpResponse {
    HttpMethod method;
    HttpProtocol protocol;
    HttpStatusCode status_code;
};

// @performance Create a cached header line for 200 responses
// @performance Create a cached header for most common response (incl. CSP, referrer, x-*, ...)

#endif