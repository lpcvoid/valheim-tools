//
// Created by lpcvoid on 03/04/2021.
//

#include <iostream>
#include <filesystem>
#include <unordered_set>
#include "World.h"

void print_portals(valheim::WorldData &world_data) {
    using POI = std::pair<std::string, valheim::Position>;


    std::vector<POI> portals;
    std::vector<POI> graves;
    for (auto &zdo : world_data.zdos) {
        //if (!zdo.data.strings.empty())
        //    std::cout << zdo.data.strings.begin()->second << " " << zdo.id << " " << zdo.data.prefab << std::endl;

        if (zdo.data.prefab == -661882940) { //this magic number is caled by GetHashCode() c# function
            if (!zdo.data.strings.empty())
                portals.emplace_back(zdo.data.strings.begin()->second, zdo.data.position);
        }

        if (zdo.data.prefab == -1273339005) {
            if (!zdo.data.strings.empty())
                graves.emplace_back(zdo.data.strings.begin()->second, zdo.data.position);
        }
    }

    //count occurrences
    std::map<std::string, uint32_t> occurrence_count;
    for (auto &portal : portals) {
        occurrence_count[portal.first]++;
    }

    //sort it by portal name so overview is nicer
    std::sort(portals.begin(), portals.end(), [&](auto &a, auto &b) {
        return std::hash<std::string>{}(a.first) > std::hash<std::string>{}(b.first);
    });

    //present results
    std::cout << "Found a total of " << portals.size() << " portals." << std::endl;
    std::cout << std::endl;
    std::cout << "Dangling portals (not connected):" << std::endl;
    std::cout << std::endl;
    std::for_each(portals.begin(), portals.end(), [&](auto portal) {
        if (occurrence_count[portal.first] == 1) {
            std::cout << portal.first << ", at (" << std::get<0>(portal.second)
                      << "," << std::get<1>(portal.second)
                      << "," << std::get<2>(portal.second) << ")" << std::endl;
        }
    });
    std::cout << std::endl;
    std::cout << "Connected portals:" << std::endl;
    std::cout << std::endl;
    std::for_each(portals.begin(), portals.end(), [&](auto portal) {
        if (occurrence_count[portal.first] == 2) {
            std::cout << portal.first << ", at (" << std::get<0>(portal.second)
                      << "," << std::get<1>(portal.second)
                      << "," << std::get<2>(portal.second) << ")" << std::endl;
        }
    });

    std::cout << std::endl;
    std::cout << "Graves:" << std::endl;
    std::cout << std::endl;
    std::for_each(graves.begin(), graves.end(), [&](auto grave) {
            std::cout << grave.first << ", at (" << std::get<0>(grave.second)
                      << "," << std::get<1>(grave.second)
                      << "," << std::get<2>(grave.second) << ")" << std::endl;

    });

}

void print_help() {

    std::cout << "Usage : " << std::endl;
    std::cout << "whereismystuff [PATH_TO_WORLDDB]" << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "Valheim stuff finder" << std::endl;
    if (1 == argc)
        print_help();
    else if (2 == argc) {
        std::string world_filepath = argv[1];
        if (std::filesystem::exists(world_filepath)) {
            //load the file
            valheim::World world;
            auto world_data = world.load_from_file(world_filepath);
            if (world_data) {
                print_portals(world_data.value());
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