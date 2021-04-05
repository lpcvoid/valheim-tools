#include <iostream>
#include <filesystem>
#include <map>
#include "Player.h"

void print_sync(std::vector<valheim::PlayerData> playerfiles) {

}

void print_help() {

    std::cout << "Usage : " << std::endl;
    std::cout << "This takes a set of character databases and synchronizes the map exploration state" << std::endl;
    std::cout << "mapdatasync [*.fcl]" << std::endl;
    std::cout << "      example: mapdatasync foo.fcl bar.fcl" << std::endl;
    std::cout << "Tool will generate N new fcl files in a new directory" << std::endl;

}

int main(int argc, char *argv[]) {
    std::cout << "Valheim mapdata synchronization" << std::endl;
    if (1 == argc)
        print_help();
    else {
        //multiple player files assumed
        std::vector<std::string> arguments(argv + 1, argv + argc);

        valheim::Player playerhandler;
        std::vector<valheim::PlayerData> list_playerdata;

        std::for_each(arguments.begin(), arguments.end(), [&](const std::string &singlefile) {
            if (!std::filesystem::exists(singlefile)) {
                std::cerr << "File " << singlefile << " not found!" << std::endl;
                std::exit(1);
            } else {
                auto ret = playerhandler.load_from_file(singlefile);
                if (ret) {
                    std::cout << "Loaded player " << ret->playername << std::endl;
                    list_playerdata.emplace_back(ret.value());
                } else {
                    std::cerr << "Failed to read " << singlefile << ", exiting without writing anything" << std::endl;
                    std::exit(2);
                }
            }
        });

        for (auto &player : list_playerdata) {
            playerhandler.save_to_file(player, "mapsync_output/" + player.playername + ".fch_presync");
        }


        std::map<uint64_t, valheim::PlayerMapExploreData> map_accumulator;
        for (auto &player : list_playerdata) {
            for (auto &map : player.worlds) {

                if (!map.map_data.has_value()) {
                    std::cout << "Map " << map.key << " has no map data attached, skipping" << std::endl;
                }

                if (!map_accumulator.contains(map.key)) {
                    map_accumulator[map.key] = map.map_data->explored; //use first occurance as start point
                }

                valheim::PlayerMapExploreData &accumulator = map_accumulator[map.key];
                if (map.map_data->explored.size() != accumulator.size()) {
                    std::cerr << "One map differed in size from the rest: " << player.playername << ", key "
                              << map.key << " - not handling" << std::endl;
                } else {
                    for (int i = 0; i < accumulator.size(); ++i) {
                        accumulator[i] |= map.map_data->explored[i];
                    }
                    std::cout << "Synced map progress of " << player.playername << " in world " << map.key
                              << ", which is " << map.map_data->calculate_explored_percentage() << "% explored."
                              << std::endl;
                }
            }
        }
        std::filesystem::create_directory("mapsync_output");

        std::cout << "Synchronizing and exporting player files..." << std::endl;
        for (auto &player : list_playerdata) {
            for (auto &map : player.worlds) {
                map.map_data->explored = map_accumulator[map.key];
                std::cout << "Exporting " << player.playername << " with world " << map.key << ", now explored by "
                          << map.map_data->calculate_explored_percentage() << " percent." << std::endl;
            }
            std::string lowercase_name = player.playername;

            std::transform(lowercase_name.begin(), lowercase_name.end(), lowercase_name.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            playerhandler.save_to_file(player, "mapsync_output/" + lowercase_name + ".fch");
        }


        std::cout << "Done!" << std::endl;
    }
}
