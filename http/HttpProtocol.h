/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_JINGGA_HTTP_PROTOCOL_H
#define COMS_JINGGA_HTTP_PROTOCOL_H

#include "../stdlib/Types.h"

enum HttpProtocol : byte {
    HTTP_PROTOCOL_UNKNOWN,
    HTTP_PROTOCOL_1_1,
    HTTP_PROTOCOL_2,
    HTTP_PROTOCOL_3,
};

#endif