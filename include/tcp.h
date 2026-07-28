#pragma once
#include <cstdint>
#include "ip.h"
#include "config_loader.h"

#pragma pack(push,1)

struct tcp_hdr{
    std::uint16_t src_port;
    std::uint16_t dst_port;

    std::uint32_t seq;
    std::uint32_t ack;

    std::uint8_t rsv : 4;
    std::uint8_t offset : 4;

    //flags 별로 나눠야할 수도 있음 (syn ack rst같은게 여기 있던 것 같다.)
    //rst는 0x04
    std::uint8_t flags;
    std::uint16_t window;
    std::uint16_t checksum;
    std::uint16_t urgent_ptr;

};

#pragma pack(pop)

#pragma pack(push,1)

struct tcp_rst_packet{
    ip_hdr iphdr;
    tcp_hdr tcphdr;
};

#pragma pack(pop)

#pragma pack(push,1)

struct tcp_pseudo_hdr{
    std::uint32_t src_address;
    std::uint32_t dst_address;
    std::uint8_t zero;
    std::uint8_t protocol;
    std::uint16_t length;
};

#pragma pack(pop)

std::uint16_t tcp_checksum(tcp_hdr* packet,tcp_pseudo_hdr* tcp_pseudo,int len);

bool detect_port(tcp_hdr* tcphdr,guard9_config* config);