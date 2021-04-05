#include <iostream>
#include <filesystem>
#include "Player.h"

void print_sync(const valheim::PlayerData &player) {
    std::cout << "Playername: " << player.playername << std::endl;
    std::cout << "Player id: " << player.player_id << std::endl;
    std::cout << "Start seed: " << player.start_seed << std::endl;

    std::cout << "Kills: " << player.kills << std::endl;
    std::cout << "Builds: " << player.builds << std::endl;
    std::cout << "Crafts: " << player.crafts << std::endl;
    std::cout << "Deaths: " << player.deaths << std::endl;
    std::cout << "Player data size: " << player.player_data.size() << std::endl;

    std::cout << "Found " << player.worlds.size() << " Worlds:" << std::endl;
    std::for_each(player.worlds.begin(), player.worlds.end(), [&](const valheim::PlayerWorldData &world) {
        std::cout << "  World Key: " << world.key << std::endl;
        std::cout << "  Home base position: " << std::get<0>(world.home_point)
                  << "," << std::get<1>(world.home_point)
                  << "," << std::get<2>(world.home_point) << std::endl;
        if (world.has_death_point){
            std::cout << "  Death position: " << std::get<0>(world.death_point)
                      << "," << std::get<1>(world.death_point)
                      << "," << std::get<2>(world.death_point) << std::endl;
        }

        if (world.has_logout_point){
            std::cout << "  Logout position: " << std::get<0>(world.logout_point)
                      << "," << std::get<1>(world.logout_point)
                      << "," << std::get<2>(world.logout_point) << std::endl;
        }

        if (world.has_custom_spwan_point){
            std::cout << "  Custom spawn position: " << std::get<0>(world.custom_spawn_point)
                      << "," << std::get<1>(world.custom_spawn_point)
                      << "," << std::get<2>(world.custom_spawn_point) << std::endl;
        }

        if (world.map_data.has_value()){
            std::cout << "  Map exploration percentage: " << world.map_data->calculate_explored_percentage() << std::endl;

            std::cout << "  " << world.map_data->pins.size() << " pins found" << std::endl;
            for (auto pin : world.map_data->pins){
                std::cout << "    " <<  pin.name << std::get<0>(world.custom_spawn_point)
                                      << "," << std::get<1>(world.custom_spawn_point)
                                      << "," << std::get<2>(world.custom_spawn_point) << std::endl;
            }
        }


    });
}

void print_help() {

    std::cout << "Usage : " << std::endl;
    std::cout << "playerdb_reader [PATH_TO_PLAYERDB]" << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "Valheim player db reader" << std::endl;
    if (1 == argc)
        print_help();
    else if (2 == argc) {
        std::string playerdb_filepath = argv[1];
        if (std::filesystem::exists(playerdb_filepath)) {
            //load the file
            valheim::Player player;
            auto playerdata = player.load_from_file(playerdb_filepath);
            if (playerdata){
                print_sync(playerdata.value());
                //test saving
                player.save_to_file(playerdata.value(), std::filesystem::path(playerdb_filepath).parent_path().generic_string() + "/writetest.fcl");
            }
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
