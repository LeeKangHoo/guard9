#include "ip.h"

std::uint16_t ip_checksum(ip_hdr* iphdr,int len){
    std::uint8_t* cursor = reinterpret_cast<std::uint8_t*>(iphdr);
    std::uint32_t cur_sum = 0;
    for (int i = 0; i < len*2;i++){ 
        std::uint16_t word = (static_cast<std::uint16_t>(cursor[0]) << 8) | static_cast<std::uint16_t>(cursor[1]);
        cur_sum += word;
        cursor += 2;
    }
    while (cur_sum >> 16){
        cur_sum = (cur_sum & 0xffff) + (cur_sum >> 16);
    }
    std::uint16_t checksum = ~(static_cast<std::uint16_t>(cur_sum));
    return htons(checksum);
}
