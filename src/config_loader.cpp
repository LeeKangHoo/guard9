#include "config_loader.h"
#include <fstream>
#include <iostream>


int config_load(guard9_config* config){

    std::ifstream file("config/target.txt");
    if(!file.is_open()){
        std::cout << "target.txt open failed" << std::endl;

    }

    std::string domain;
    while(std::getline(file,domain)){
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
    return 0;
}
