#include "pch.h"
#include "network_engine.h"
#include "config_loader.h"





int main(){
    guard9_config config{};

    config_load(&config);
    start_nfq_loop(&config);

    return 0;
}
