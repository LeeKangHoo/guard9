#pragma once
#include "pch.h"
#include "config_loader.h"


struct cb_context{
    guard9_config* config;
    int raw_socket_fd;
};

int cb(nfq_q_handle* q,nfgenmsg* message, nfq_data* packet, void* user_data);
int start_nfq_loop(guard9_config* config);