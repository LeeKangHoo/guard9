#include "network_engine.h"
#include "ip.h"
#include "tcp.h"
#include "tls.h"

int start_nfq_loop(guard9_config* config){
    cb_context context{};
    context.config = config;
    nfq_handle* handle = nfq_open();
    if (handle == nullptr) {
        std::cout << "handle create failed" << std::endl;
        return 1;
    }
    
    nfq_q_handle* q = nfq_create_queue(handle,0,cb,&context);

    if (q == nullptr) {
        nfq_close(handle);
        std::cout << "queue create failed" << std::endl;
        return 1;
    }

    int result = nfq_set_mode(q,NFQNL_COPY_PACKET,0xffff);
    if (result < 0){
        nfq_destroy_queue(q);
        nfq_close(handle);
        std::cout << "nfq set mode failed" << std::endl;
        return 1;
    }

    int fd = nfq_fd(handle);
    if (fd < 0) {
        nfq_destroy_queue(q);
        nfq_close(handle);
        return 1;
    }

    context.raw_socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (context.raw_socket_fd < 0){
        std::cout << "socket error" << std::endl;
    }
        
    char buffer[65536];
    
    while (true){
        ssize_t received = recv(fd,buffer,sizeof(buffer),0);
        if (received <= 0){
            std::cout << "recv error" << std::endl;
            break;
        }
        nfq_handle_packet(handle,buffer,received);
        
    }

    nfq_destroy_queue(q);
    nfq_close(handle);
    close(context.raw_socket_fd);
    return 0;
}

int cb(nfq_q_handle* q,nfgenmsg* message, nfq_data* packet, void* user_data){
    unsigned char* payload = nullptr;
    cb_context* context = static_cast<cb_context*>(user_data);

    nfqnl_msg_packet_hdr* hdr = nfq_get_msg_packet_hdr(packet);
    if (hdr == nullptr) {
        std::cout << "hdr is null" << std::endl;
        return -1;
    }
    std::uint32_t packet_id = ntohl(hdr->packet_id);
    // std::cout << "packet received" << std::endl;
    // std::cout << "packet_id:" << packet_id << std::endl;

    int payload_length = nfq_get_payload(packet,&payload);

    if (payload_length <= 0){
        return nfq_set_verdict(q,packet_id,NF_ACCEPT,0,nullptr);
    }


    ip_hdr iphdr;
    std::memcpy(&iphdr, payload,sizeof(iphdr));
    int ip_length = iphdr.ihl * 4;

    if (iphdr.version != 4){
        return nfq_set_verdict(q,packet_id,NF_ACCEPT,0,nullptr);
    }

    tcp_hdr tcphdr;
    std::memcpy(&tcphdr, payload+ip_length,sizeof(tcphdr));
    int tcp_length = tcphdr.offset * 4;

    if (payload_length - ip_length - tcp_length <= 5){
        return nfq_set_verdict(q,packet_id,NF_ACCEPT,0,nullptr);
    }



    // tls 함수 (payload 길이, payload) 
    // target.txt안에 있는 dns 탐지하고 판별하는 함수
    int result = detect_sni(payload_length-ip_length-tcp_length,payload+ip_length+tcp_length,context->config);
    if (result == 1){

        // rst 패킷 보내기 위해 직접 만들어서 보내는 구간

        tcp_rst_packet tcp_rst{};
        tcp_rst.iphdr.ihl = 5;
        tcp_rst.iphdr.version = 4;
        tcp_rst.iphdr.tos = iphdr.tos;
        tcp_rst.iphdr.length = htons(40);
        tcp_rst.iphdr.id = 0;
        // 0x4000 Don't fragment
        tcp_rst.iphdr.offset = htons(0x4000);
        tcp_rst.iphdr.ttl = 65;
        tcp_rst.iphdr.protocol = 6;
        tcp_rst.iphdr.checksum = 0;
        tcp_rst.iphdr.src_address = iphdr.dst_address; 
        tcp_rst.iphdr.dst_address = iphdr.src_address;
        tcp_rst.iphdr.checksum = ip_checksum(&tcp_rst.iphdr,tcp_rst.iphdr.ihl);

        tcp_rst.tcphdr.src_port = tcphdr.dst_port;
        tcp_rst.tcphdr.dst_port = tcphdr.src_port;
        tcp_rst.tcphdr.seq = tcphdr.ack;
        tcp_rst.tcphdr.ack = 0;
        tcp_rst.tcphdr.rsv = 0;
        tcp_rst.tcphdr.offset = 5;
        tcp_rst.tcphdr.flags = 0x04;
        tcp_rst.tcphdr.window = 0;
        tcp_rst.tcphdr.checksum = 0;
        tcp_rst.tcphdr.urgent_ptr = 0;

        tcp_pseudo_hdr tcp_pseudo{};
        tcp_pseudo.src_address = iphdr.dst_address;
        tcp_pseudo.dst_address = iphdr.src_address;
        tcp_pseudo.zero = 0;
        tcp_pseudo.protocol = 6;
        tcp_pseudo.length = htons(20);

        tcp_rst.tcphdr.checksum = tcp_checksum(&tcp_rst.tcphdr,&tcp_pseudo,tcp_rst.tcphdr.offset);


        sockaddr_in destination{};
        destination.sin_family = AF_INET;
        destination.sin_port = 0;
        destination.sin_addr.s_addr = iphdr.src_address;
        
        ssize_t sent = sendto(context->raw_socket_fd,&tcp_rst,sizeof(tcp_rst),0,reinterpret_cast<const sockaddr*>(&destination),sizeof(destination));
        if (sent < 0){
            std::cout << "sendto error" << std::endl;
        }


        return nfq_set_verdict(q,packet_id,NF_DROP,0,nullptr);
        // return nfq_set_verdict(q,packet_id,NF_DROP,0,nullptr);
    }else if (result == 0){
        return nfq_set_verdict(q,packet_id,NF_ACCEPT,0,nullptr);
    }else{
        //std::cout << "verdict error" << std::endl;
        // return nfq_set_verdict(q,packet_id,NF_DROP,0,nullptr);
    }


    return nfq_set_verdict(q,packet_id,NF_ACCEPT,0,nullptr);
}
