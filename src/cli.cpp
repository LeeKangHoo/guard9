#include "cli.h"

#include <sstream>
#include <fstream>
#include <algorithm>


constexpr const char* guard9_logo = R"(
                           _  _____ 
                          | ||  _  |
  __ _ _   _  __ _ _ __ __| || |_| |
 / _` | | | |/ _` | '__/ _` |\____ |
| (_| | |_| | (_| | | | (_| |.___/ /
 \__, |\__,_|\__,_|_|  \__,_|\____/ 
  __/ |                             
 |___/                              )";

void wrong_command(){
    std::cout << std::endl;
    std::cout << "wrong command!" << "\ninput enter..." << std::endl;
    std::string pause;
    std::getline(std::cin, pause);
}

void save_domains(std::vector<std::string>* domains){
    std::ofstream file("config/target.txt");
    
    if (!file.is_open()){
        std::cout << "target.txt open failed" << std::endl;
    }

    for (const std::string& domain : *domains){
        file << domain << '\n';
    }

    file.close();

}

int manage_domains(guard9_config* config)
{
    int page = 0;
    while(true){
        std::cout << "\033[2J\033[H" << std::flush;
        std::cout << "current blocked domains" <<std::endl;
        for (int i = 0; i < 10; i++){
            int cur = page * 10 + i;
            if (config->domains.size()<=cur){
                break;
            }
            std::cout << cur+1 << ". " << config->domains[cur] << std::endl;
        }

        std::cout << std::endl;
        std::cout << "next : n, previous : p, quit : q" << std::endl;
        std::cout << std::endl;
        std::cout << "intput : ";
        std::string input;
        std::string command;
        std::string first;
        std::string extra;
        std::getline(std::cin,input);

        std::istringstream cmd(input);
        if (!(cmd >> command)){ // 아무 입력이 없을 때 
            wrong_command();
            continue;
        }
        if (!(cmd >> first)){ // 이러면 하나도 입력안했을때는 못잡음 아니지 add naver.com / q
            if (command == "q"){
                return 0;
            }
            else if (command == "n"){
                // 1 -> 2
                // 23 / 1
                // 20  
                if ((page+1)*10 < config->domains.size()){
                    page++;
                }
                continue;
            }
            else if (command == "p")
            {
                if (page > 0){
                    page--;
                }
                continue;
            }
            wrong_command();
            continue;
        }
        else if (cmd >> extra){
            wrong_command();
            continue;
        }

        if (command == "add"){
            //추가 로직
            bool inserted = false;
            {
                std::lock_guard<std::mutex> domain_lock(config->config_mtx);
                inserted = config->blocked_domains.insert(first).second;
                if (inserted){
                    config->domains.push_back(first);
                }
            }
            if (!inserted){
                std::cout << "domain is already exist" << std::endl;
                std::string tmp;
                std::getline(std::cin,tmp);
                continue;
            }
            
            
            save_domains(&config->domains);
        } 
        else if(command == "remove") {
            // 삭제 로직
            std::vector<std::string>::iterator find_result;
            bool removed = false;
            {
                std::lock_guard<std::mutex> domain_lock(config->config_mtx);
                find_result = std::find(config->domains.begin(),config->domains.end(),first);
                if (find_result != config->domains.end()){
                    config->blocked_domains.erase(first);
                    config->domains.erase(find_result);
                    removed = true;
                }
            }
            if (!removed){
                std::cout << "can't found domain" << std::endl;
                std::string tmp;
                std::getline(std::cin,tmp);
                continue;
            }

            save_domains(&config->domains);
        } 
        else if (command == "q") {
            return 0;
        }
        else {
            wrong_command();
            continue;
        }
    }

    return 0;
}


int start_cli(guard9_config* config){
    while (true){
        std::cout << "\033[2J\033[H" << std::flush;
        std::cout << guard9_logo << std::endl;
        std::cout << "1. manage block domain" <<std::endl;
        for (int i = 0;i < 5 ; i++){
        std::cout << std::endl;
        }
        std::cout << "intput : ";

        std::string input;
        std::string command;
        std::string extra;

        std::getline(std::cin,input);

        std::istringstream cmd(input);

        if (!(cmd >> command) || cmd >> extra){
            wrong_command();
            continue;
        }

        if (command == "1"){
            manage_domains(config);
        }else{
            wrong_command();
            continue;
        }

    }

    return 0;
}