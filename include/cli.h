#pragma once
#include "config_loader.h"
#include "pch.h"

void wrong_command();

void save_domains(std::vector<std::string>* domains);

int manage_domains(guard9_config* config);

int start_cli(guard9_config* config);