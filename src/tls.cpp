#include "tls.h"

int detect_sni(std::size_t payload_length, unsigned char* payload,std::unordered_set<std::string>* blocked_domains){
    unsigned char* cursor = payload;
    unsigned char* end = payload + payload_length;

    tls_record record{};
    std::memcpy(&record, cursor, sizeof(record));

    if (record.type != 0x16){
        return -1;
    }
    cursor += sizeof(record);

    tls_client_hello client_hello{};
    std::memcpy(&client_hello, cursor, sizeof(client_hello));

    if (client_hello.type != 0x01){
        return -1;
    }
    cursor += sizeof(client_hello);

    std::uint8_t session_id_length = cursor[0];
    cursor += 1 + session_id_length;

    std::uint16_t cipher_suite_length;
    std::memcpy(&cipher_suite_length,cursor,sizeof(cipher_suite_length));
    cipher_suite_length = ntohs(cipher_suite_length);
    cursor += 2 + cipher_suite_length;

    std::uint8_t compression_method_length = cursor[0];
    cursor += 1 + compression_method_length;

    std::uint16_t extensions_length;
    std::memcpy(&extensions_length,cursor,sizeof(extensions_length));
    extensions_length = ntohs(extensions_length);
    cursor += 2;

    unsigned char* extensions_end = cursor + extensions_length;
    
    while (cursor < extensions_end){
        std::uint16_t ex_type;
        std::memcpy(&ex_type, cursor, sizeof(ex_type));
        ex_type = ntohs(ex_type);

        std::uint16_t ex_length;
        std::memcpy(&ex_length, cursor+2,sizeof(ex_length));
        ex_length = ntohs(ex_length);
        
        if (ex_type != 0x00){
            cursor += 4 + ex_length;
            continue;
        }
        cursor += 4;

        std::uint16_t snll; // server name list length
        std::memcpy(&snll,cursor,sizeof(snll));
        snll = ntohs(snll);
        cursor += 3; //server name type (1 byte)는 지금은 항상 host name(0)이라 그냥 건너뛰기

        std::uint16_t snl; // server name length
        std::memcpy(&snl, cursor, sizeof(snl));
        snl = ntohs(snl);
        cursor += 2;

        std::string server_name(reinterpret_cast<char*>(cursor),snl);
        // 저장해둔 dns와 같은지 비교 하는 로직
        // 임시로 naver.com로 테스트
        // if (server_name == "naver.com" || server_name == "www.naver.com"){
        //     return 1;
        // }
        if (blocked_domains->find(server_name) != blocked_domains->end()){
            std::cout << server_name << " is detected!!" << std::endl;
            return 1;
        }
        
        return 0;
        
    }
    return 0;

}
