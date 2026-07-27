#pragma once
#include "pch.h"
#include <fstream>

struct guard9_config{
    std::mutex config_mtx;
    
    std::unordered_set<std::string> blocked_domains;
    std::vector<std::string> domains;

    std::unordered_set<std::uint32_t> blocked_ips;
    std::vector<std::string> ips;
};

int config_load(guard9_config* config);