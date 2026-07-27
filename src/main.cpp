#include "pch.h"
#include "network_engine.h"
#include "config_loader.h"
#include "cli.h"

#include <thread>

//                               _  _____ 
//                              | ||  _  |
//      __ _ _   _  __ _ _ __ __| || |_| |
//     / _` | | | |/ _` | '__/ _` |\____ |
//    | (_| | |_| | (_| | | | (_| |.___/ /
//     \__, |\__,_|\__,_|_|  \__,_|\____/ 
//      __/ |                             
//     |___/                              



int main(){
    guard9_config config{};

    config_load(&config);
    std::thread network_thread(start_nfq_loop,&config);

    start_cli(&config);


    

    return 0;
}
