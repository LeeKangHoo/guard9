#include "tcp.h"



std::uint16_t tcp_checksum(tcp_hdr* packet,tcp_pseudo_hdr* tcp_pseudo,int len){
    // cursor는 pseudo haeder쪽 먼저 할당하고 계산 끝나면 tcp header로 다시 할당하기
    std::uint8_t* cursor = reinterpret_cast<std::uint8_t*>(tcp_pseudo);
    std::uint32_t cur_sum = 0;
    for (int i = 0; i < sizeof(tcp_pseudo_hdr)/2; i++){
        std::uint16_t word = (static_cast<std::uint16_t>(cursor[0]) << 8) | static_cast<std::uint16_t>(cursor[1]);
        cur_sum += word;
        cursor += 2;
    }
    cursor = reinterpret_cast<std::uint8_t*>(packet);

    for (int i = 0; i < len*2; i++){
        std::uint16_t word = (static_cast<std::uint16_t>(cursor[0]) << 8) | static_cast<std::uint16_t>(cursor[1]);
        cur_sum += word;
        cursor += 2;
    }
    while(cur_sum >> 16){
        cur_sum = (cur_sum & 0xffff) + (cur_sum >> 16);
    }

    std::uint16_t checksum = ~(static_cast<std::uint16_t>(cur_sum));
    return htons(checksum);

}
