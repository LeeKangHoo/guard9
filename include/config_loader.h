#pragma once
#include "pch.h"
#include <fstream>

struct guard9_config{
    std::mutex config_mtx;
    
    std::unordered_set<std::string> blocked_domains;
    std::vector<std::string> domains;

    std::unordered_set<std::uint32_t> blocked_ips;
    std::vector<std::string> ips;

    std::unordered_set<std::uint16_t> blocked_ports;
    std::vector<std::string> ports;
};

int config_load(guard9_config* config);