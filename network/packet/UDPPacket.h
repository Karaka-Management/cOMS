#ifndef TOS_NETWORK_PACKET_H
#define TOS_NETWORK_PACKET_H

#include <stdio.h>

#include "../../stdlib/Types.h"
#include "../../compression/LZP.h"

#include "PacketHeader.h"

// The message loop is as follows:
//      Game Data                   -> pack data
//      Game Message (packed)       -> compress data
//      Game Message (compressed)   -> combine (ipv6 header + udp header + body)
//      Packet
//      ... Send ...
//      ... Receive ...
//      Packet                     -> split (ipv6 header + udp header + body)
//      Game Message (compressed)   -> decompress data
//      Game Message (packed)       -> unpack data
//      Game Data

// Game messages
//      Type 1: Snapshot = absolute state
//      Type 2: Delta = relative data to previous data
//              Can be usually much smaller data size, since deltas are usually small

struct UDPPacketIPv6 {
    byte* data;
};

struct UDPMessageIPv6 {
    HeaderIPv6Unpacked header_ipv6;
    UDPHeaderIPv6Unpacked header_udp;
    CustomHeaderUnpacked header_custom;

    size_t length;
    byte* data;
};

/**
 * WARNING: This requires the original message to remain in memory since we are only referencing the data
 */
inline
void udp_packet_to_message(const UDPPacketIPv6* packet, UDPMessageIPv6* message)
{
    unpack_ipv6_header((const HeaderIPv6*) packet->data, &message->header_ipv6);
    unpack_udp_header_ipv6((const UDPHeaderIPv6*) (packet->data + HEADER_IPV6_SIZE), &message->header_udp);
    message->data = (byte *) (packet->data + HEADER_IPV6_SIZE + HEADER_UDP_SIZE);

    // @todo transform packet data to appropriate packet type
}

/**
 * The original message can be deleted since the data is copied over
 */
inline
void message_to_udp_packet(const UDPMessageIPv6* message, UDPPacketIPv6* packet)
{
    pack_ipv6_header(&message->header_ipv6, (HeaderIPv6 *) packet);

    packet->data = packet->data + HEADER_IPV6_SIZE;
    pack_udp_header_ipv6(&message->header_udp, (UDPHeaderIPv6 *) packet);
    packet->data = packet->data - HEADER_IPV6_SIZE;

    memcpy(packet->data + HEADER_IPV6_SIZE + HEADER_UDP_SIZE, message->data, message->length);
}

inline
void decompress_data(UDPMessageIPv6* message, byte* decompress_buffer)
{
    decode_lzp(message->data, message->length, decompress_buffer);
    message->data = decompress_buffer;
}

inline
void compress_data(UDPMessageIPv6* message, byte* compressed_buffer)
{
    encode_lzp(message->data, message->length, compressed_buffer);
    message->data = compressed_buffer;
}

#endif