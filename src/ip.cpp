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

bool detect_ip(ip_hdr* iphdr,guard9_config* config){
    std::lock_guard<std::mutex> detect_lock(config->config_mtx);
    char ip[INET_ADDRSTRLEN];
    if (config->blocked_ips.find(iphdr->src_address) != config->blocked_ips.end()){
        inet_ntop(AF_INET,&iphdr->src_address,ip,sizeof(ip));
        std::cout << ip <<" is detected!!" << std::endl;
        return true;
    } 
    else if (config->blocked_ips.find(iphdr->dst_address) != config->blocked_ips.end()){
        inet_ntop(AF_INET,&iphdr->dst_address,ip,sizeof(ip));
        std::cout << ip <<" is detected!!" << std::endl;;
        return true;
    }
    return false;
}

