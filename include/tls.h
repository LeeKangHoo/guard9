#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <unordered_set>
#include <iostream>
// 단위별로 구조체 만들어 놓음 
// cursor를 nfq_get_payload()가 주는 크기 + ip크기 + tcp 해서 tls_record의 위치로 초기화
// tls record에서 content type이 handshake면 tls_client_hello에 cursor에서 6byte만큼 할당
// cursor는 +6
// legacy version 다음부터 extentions의 길이 전까지의 값은 무시하고 바로 extentions의 길이를 나타내는
// 곳까지 cursor를 옮겨야한다.
// random -> sessionID -> Cipher Suites -> compression method 순서로 있고 각 내용 전에 길이를 알려준다.
// 따라서 cursor로 이동하면서 해당 위치에 적혀있는 length만큼 다시 + 해준다.
// 3번 이동 하면 extentions의 길이가 있는 위치이기 때문에 해당 값을 읽어 저장해둠


int tls_parse(std::size_t payload_length, unsigned char* payload,std::unordered_set<std::string>* blocked_domains);

#pragma pack(push,1)
struct tls_record{
    std::uint8_t type;
    std::uint16_t version;
    std::uint16_t length;
};
#pragma pack(pop)

#pragma pack(push,1)
struct tls_client_hello{
    std::uint8_t type;
    std::uint8_t length[3];
    
    std::uint16_t version;
    std::uint8_t random[32];
};
#pragma pack(pop)


// struct tls_extentions{

// };
