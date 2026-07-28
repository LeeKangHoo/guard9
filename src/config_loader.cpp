#include "config_loader.h"
#include <fstream>


int config_load(guard9_config* config){
    //blocked_domains 
    std::ifstream domains_file("config/blocked_domains.txt");
    if(!domains_file.is_open()){
        std::cout << "blocked_domains.txt open failed" << std::endl;

    }
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

    //blocked_ips
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

    //blocked_ports
    std::ifstream ports_file("config/blocked_ports.txt");
    if(!ports_file.is_open()){
        std::cout << "blocked_ports.txt open failed" << std::endl;

    }
    std::string port;
    std::uint16_t raw_port;
    while(std::getline(ports_file,port)){
        if(!port.empty() && port.back() == '\r') {
            port.pop_back();
        }
        if (port.empty()){
        continue;
        }
        int parsed_port = std::stoi(port);
        if (parsed_port < 1 || parsed_port > 65535){
            std::cout << parsed_port << " is invalid port" << std::endl;
            continue;
        }
        raw_port = static_cast<std::uint16_t>(parsed_port);
        config->blocked_ports.insert(htons(raw_port));
        config->ports.push_back(port);
    }
    std::cout << config->blocked_ports.size() << " ports loaded" << std::endl;
    ports_file.close();



    return 0;
}
