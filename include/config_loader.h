#pragma once
#include <fstream>
#include <iostream>
#include <unordered_set>

struct guard9_config{
    std::unordered_set<std::string> blocked_domains;
};

int config_load(guard9_config* config);