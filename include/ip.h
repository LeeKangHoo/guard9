#pragma once
#include <cstdint>
#include <arpa/inet.h>

#pragma pack(push,1)

struct ip_hdr {
    // gcc에서 :4(비트 필드)와 같이 비트를 나눠 선언하면 낮은 비트부터 나열하기 때문에 ihl을 먼저 선언해야 version(4bit) -> ihl(4bit)순서대로 나열됨
    std::uint8_t ihl : 4;
    std::uint8_t version : 4;

    std::uint8_t tos;
    std::uint16_t length;
    std::uint16_t id;
    std::uint16_t offset;
    std::uint8_t ttl;
    std::uint8_t protocol;
    std::uint16_t checksum;

    std::uint32_t src_address;
    std::uint32_t dst_address;

};

#pragma pack(pop)

std::uint16_t ip_checksum(ip_hdr* iphdr,int len);
