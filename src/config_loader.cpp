#include "config_loader.h"
#include <fstream>


int config_load(guard9_config* config){

    std::ifstream domains_file("config/blocked_domains.txt");
    if(!domains_file.is_open()){
        std::cout << "blocked_domains.txt open failed" << std::endl;

    }
    //blocked_domains 
    std::string domain;
    while(std::getline(domains_file,domain)){
        if(!domain.empty() && domain.back() == '\r') {
            domain.pop_back();
        }
        if (domain.empty()){
        continue;
        }
        config->blocked_domains.insert(domain);
        config->domains.push_back(domain);
    }
    std::cout << config->blocked_domains.size() << " domains loaded" << std::endl;
    domains_file.close();

    //ip인데 얘도 열어서 파싱하는거 만들어야한다.
    std::ifstream ips_file("config/blocked_ips.txt");
    if (!ips_file.is_open()){
        std::cout << "blocked_ip.txt open failed" << std::endl;
    }

    std::string ip;
    std::uint32_t raw_ip;
    while(std::getline(ips_file,ip)){
        if(!ip.empty() && ip.back() == '\r') {
            ip.pop_back();
        }
        if (ip.empty()){
        continue;
        }
        inet_pton(AF_INET,ip.c_str(),&raw_ip);
        config->blocked_ips.insert(raw_ip);
        config->ips.push_back(ip);
    }
    std::cout << config->blocked_ips.size() << " ips loaded" << std::endl;
    ips_file.close();
    return 0;
}
