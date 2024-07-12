#ifndef TOS_NETWORK_PACKET_HEADER_H
#define TOS_NETWORK_PACKET_HEADER_H

#include <stdio.h>

#include "../../stdlib/Types.h"

#define HEADER_IPV6_SIZE 40
// Size 40 bytes
struct HeaderIPv6 {
    byte data[HEADER_IPV6_SIZE];
};

// Size 42 bytes
struct HeaderIPv6Unpacked {
    byte version;
    byte traffic_class;
    uint32 flow_label;

    uint16 length;
    byte next_header;
    byte hop_limit;

    byte src[16];
    byte dst[16];
};

inline
void unpack_ipv6_header(const HeaderIPv6* ipv6, HeaderIPv6Unpacked* ipv6_unpacked)
{
    ipv6_unpacked->version       = (ipv6->data[0] >> 4) & 0x0F;
    ipv6_unpacked->traffic_class = ((ipv6->data[0] & 0x0F) << 4) | (ipv6->data[1] >> 4);
    ipv6_unpacked->flow_label    = ((ipv6->data[1] & 0x0F) << 16) | (ipv6->data[2] << 8) | ipv6->data[3];
    ipv6_unpacked->length        = (ipv6->data[4] << 8) | ipv6->data[5];
    ipv6_unpacked->next_header   = ipv6->data[6];
    ipv6_unpacked->hop_limit     = ipv6->data[7];

    memcpy(ipv6_unpacked->src, &ipv6->data[8], 16);
    memcpy(ipv6_unpacked->dst, &ipv6->data[24], 16);
}

inline
void pack_ipv6_header(const HeaderIPv6Unpacked* ipv6_unpacked, HeaderIPv6* ipv6)
{
    ipv6->data[0]  = (ipv6_unpacked->version << 4) | (ipv6_unpacked->traffic_class >> 4);
    ipv6->data[1]  = (ipv6_unpacked->traffic_class << 4) | ((ipv6_unpacked->flow_label >> 16) & 0x0F);
    ipv6->data[1] |= (ipv6_unpacked->flow_label >> 16) & 0x0F;
    ipv6->data[2]  = (ipv6_unpacked->flow_label >> 8) & 0xFF;
    ipv6->data[3]  = ipv6_unpacked->flow_label & 0xFF;
    ipv6->data[4]  = (ipv6_unpacked->length >> 8) & 0xFF;
    ipv6->data[5]  = ipv6_unpacked->length & 0xFF;
    ipv6->data[6]  = ipv6_unpacked->next_header;
    ipv6->data[7]  = ipv6_unpacked->hop_limit;

    memcpy(&ipv6->data[8], ipv6_unpacked->src, 16);
    memcpy(&ipv6->data[24], ipv6_unpacked->dst, 16);
}

#define HEADER_UDP_SIZE 8
// Size 8 bytes
struct UDPHeaderIPv6 {
    byte data[HEADER_UDP_SIZE];
};

// Size 8 bytes
struct UDPHeaderIPv6Unpacked {
    uint16 src_port;
    uint16 dst_port;
    uint16 length;
    uint16 checksum;
};

inline
void unpack_udp_header_ipv6(const UDPHeaderIPv6* ipv6, UDPHeaderIPv6Unpacked* udp_unpacked)
{
    udp_unpacked->src_port = (ipv6->data[0] << 8) | ipv6->data[1];
    udp_unpacked->dst_port = (ipv6->data[2] << 8) | ipv6->data[3];
    udp_unpacked->length   = (ipv6->data[4] << 8) | ipv6->data[5];
    udp_unpacked->checksum = (ipv6->data[6] << 8) | ipv6->data[7];
}

inline
void pack_udp_header_ipv6(const UDPHeaderIPv6Unpacked* udp_unpacked, UDPHeaderIPv6* ipv6)
{
    ipv6->data[0] = (udp_unpacked->src_port >> 8) & 0xFF;
    ipv6->data[1] = udp_unpacked->src_port & 0xFF;
    ipv6->data[2] = (udp_unpacked->dst_port >> 8) & 0xFF;
    ipv6->data[3] = udp_unpacked->dst_port & 0xFF;
    ipv6->data[4] = (udp_unpacked->length >> 8) & 0xFF;
    ipv6->data[5] = udp_unpacked->length & 0xFF;
    ipv6->data[6] = (udp_unpacked->checksum >> 8) & 0xFF;
    ipv6->data[7] = udp_unpacked->checksum & 0xFF;
}

// Size 7 bytes
struct CustomHeaderUnpacked {
    uint16 msg_sequence;
    uint16 msg_ack_sequence;
    uint16 msg_ack;
    byte msg_type;
};

#endif