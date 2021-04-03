//
// Created by lpcvoid on 03/04/2021.
//

#include <fstream>
#include "World.h"
#include "Helpers.h"

std::optional<valheim::WorldData> valheim::World::load_from_file(std::filesystem::path filepath) {

    std::ifstream filestream(filepath, std::ios::binary | std::ios::ate);

    auto fileend = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
    auto filesize = fileend - filestream.tellg();

    WorldData world;

    filestream.seekg(0, std::ios::beg);
    int32_t file_version = 0;
    filestream.read(reinterpret_cast<char *>(&file_version), 4);

    world.name = read_encoded_string(filestream);
    world.seed_name = read_encoded_string(filestream);
}
