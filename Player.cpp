//
// Created by lpcvoid on 21/03/2021.
//

#include <fstream>
#include "Player.h"
#include "Helpers.h"
#include <algorithm>

std::optional<valheim::PlayerData> valheim::Player::load_from_file(std::filesystem::path filepath) {

    std::ifstream filestream(filepath, std::ios::binary | std::ios::ate);

    auto fileend = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
    auto filesize = fileend - filestream.tellg();

    PlayerData player;

    filestream.seekg(4, std::ios::beg);
    int32_t file_version = 0;
    filestream.read(reinterpret_cast<char *>(&file_version), 4);

    if (file_version >= 28) {
        filestream.read(reinterpret_cast<char *>(&player.kills), 4);
        filestream.read(reinterpret_cast<char *>(&player.deaths), 4);
        filestream.read(reinterpret_cast<char *>(&player.crafts), 4);
        filestream.read(reinterpret_cast<char *>(&player.builds), 4);
    }

    int32_t world_count = 0;
    filestream.read(reinterpret_cast<char *>(&world_count), 4);
    player.worlds.resize(world_count);
    std::for_each(player.worlds.begin(), player.worlds.end(), [&](PlayerWorldData& world){
        filestream.read(reinterpret_cast<char *>(&world.key), 8);
        filestream.read(reinterpret_cast<char *>(&world.has_custom_spwan_point), 1);
        filestream.read(reinterpret_cast<char *>(&world.custom_spawn_point), 12);
        filestream.read(reinterpret_cast<char *>(&world.has_logout_point), 1);
        filestream.read(reinterpret_cast<char *>(&world.logout_point), 12);
        if (file_version >= 30){
            filestream.read(reinterpret_cast<char *>(&world.has_death_point), 1);
            filestream.read(reinterpret_cast<char *>(&world.death_point), 12);
        }
        filestream.read(reinterpret_cast<char *>(&world.home_point), 12);
        bool has_mapdata = false;
        filestream.read(reinterpret_cast<char *>(&has_mapdata), 1);
        if ((file_version >= 29) && has_mapdata){
            int32_t map_data_size = 0;
            filestream.read(reinterpret_cast<char *>(&map_data_size), 4);
            world.map_data.resize(map_data_size);
            filestream.read(reinterpret_cast<char *>(world.map_data.data()), map_data_size);
        }
    });

    player.playername =  read_encoded_string(filestream);
    filestream.read(reinterpret_cast<char *>(&player.player_id), 8);
    player.start_seed =  read_encoded_string(filestream);
    bool has_player_data = false;
    filestream.read(reinterpret_cast<char *>(&has_player_data), 1);
    if (has_player_data){
        int32_t player_data_size = 0;
        filestream.read(reinterpret_cast<char *>(&player_data_size), 4);
        player.player_data.resize(player_data_size);
        filestream.read(reinterpret_cast<char *>(player.player_data.data()), player_data_size);
    }
    return player;
}
