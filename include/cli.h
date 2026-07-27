#pragma once
#include "config_loader.h"
#include "pch.h"

void wrong_command();

void save_file(std::vector<std::string>* domains,std::string file_path);

int manage_domains(guard9_config* config);

int manage_ips(guard9_config* config);

int start_cli(guard9_config* config);