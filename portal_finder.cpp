//
// Created by lpcvoid on 03/04/2021.
//

#include <iostream>
#include <filesystem>
#include "World.h"

void print_world_data(valheim::WorldData& world_data){

}

void print_help() {

    std::cout << "Usage : " << std::endl;
    std::cout << "portal_finder [PATH_TO_WORLDDB]" << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "Valheim portal finder" << std::endl;
    if (1 == argc)
        print_help();
    else if (2 == argc) {
        std::string world_filepath = argv[1];
        if (std::filesystem::exists(world_filepath)) {
            //load the file
            valheim::World world;
            auto world_data = world.load_from_file(world_filepath);
            if (world_data){
                print_world_data(world_data.value());
            } else {
                std::cerr << "Failed to parse supplied file" << std::endl;
            }
        } else {
            std::cerr << "File not found!" << std::endl;
        }
    } else {
        std::cerr << "Too many arguments!" << std::endl;
        print_help();
    }
}