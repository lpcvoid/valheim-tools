#include <iostream>
#include <filesystem>
#include "Player.h"

void print_player(const valheim::PlayerData& player){
    std::cout << "Kills: " <<  player.kills << std::endl;
    std::cout << "Builds: " << player.builds << std::endl;
    std::cout << "Crafts: " << player.crafts << std::endl;
    std::cout << "Deaths: " << player.deaths << std::endl;

}

void print_help(){

    std::cout << "Usage : " << std::endl;
    std::cout << "playerdb_reader [PATH_TO_PLAYERDB]" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "Valheim player db reader" << std::endl;
    if (1 == argc)
        print_help();
    else if (2 == argc ){
        std::string playerdb_filepath = argv[1];
        if (std::filesystem::exists(playerdb_filepath)){
            //load the file
            valheim::Player player;
            auto playerdata = player.load_from_file(playerdb_filepath);
            if (playerdata)
                print_player(playerdata.value());
            else
                std::cerr << "Failed to read file." << std::endl;
        } else {
            std::cerr << "File not found!" << std::endl;
        }
    } else {
        std::cerr << "Too many arguments!" << std::endl;
        print_help();
    }
}
