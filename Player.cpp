//
// Created by lpcvoid on 21/03/2021.
//

#include <fstream>
#include "Player.h"
#include "Helpers.h"
#include <algorithm>



std::optional<valheim::PlayerData> valheim::Player::load_from_file(std::filesystem::path filepath) {

    std::ifstream filestream(filepath, std::ios::binary | std::ios::ate);
    PlayerData player;
    filestream.seekg(4, std::ios::beg);
    player.file_version = buffer_read<uint32_t>(filestream);
    //filestream.read(reinterpret_cast<char *>(&player.file_version), 4);

    if (player.file_version >= 28) {
        player.kills = buffer_read<int32_t>(filestream);
        player.deaths = buffer_read<int32_t>(filestream);
        player.crafts = buffer_read<int32_t>(filestream);
        player.builds = buffer_read<int32_t>(filestream);
    }

    int32_t world_count = buffer_read<int32_t>(filestream);
    player.worlds.resize(world_count);
    std::for_each(player.worlds.begin(), player.worlds.end(), [&](PlayerWorldData& world){
        world.key = buffer_read<uint64_t>(filestream);
        world.has_custom_spwan_point = buffer_read<bool>(filestream);
        world.custom_spawn_point = buffer_read<Position>(filestream);
        world.has_logout_point = buffer_read<bool>(filestream);
        world.logout_point = buffer_read<Position>(filestream);
        if (player.file_version >= 30){
            world.has_death_point = buffer_read<bool>(filestream);
            world.death_point = buffer_read<Position>(filestream);
        }
        world.home_point = buffer_read<Position>(filestream);
        bool has_mapdata  = buffer_read<bool>(filestream);
        if ((player.file_version >= 29) && has_mapdata){
            filestream.ignore(4); //length of data array which we parse directly here
            PlayerMapData map_data;
            map_data.map_version = buffer_read<int32_t>(filestream);
            map_data.texture_size = buffer_read<int32_t>(filestream);
            map_data.explored.resize(map_data.texture_size * map_data.texture_size);
            filestream.read(reinterpret_cast<char *>(map_data.explored.data()), map_data.explored.size());
            if (map_data.map_version >= 2){
                int32_t pincount = buffer_read<int32_t>(filestream);
                map_data.pins.resize(pincount);
                for (auto& pin : map_data.pins){
                    pin.name = read_encoded_string(filestream);
                    pin.pos = buffer_read<Position>(filestream);
                    pin.pin_type = buffer_read<int32_t>(filestream);
                    pin.checked = buffer_read<bool>(filestream);
                }
            }
            if (map_data.map_version >= 4){
                map_data.pos_public = buffer_read<bool>(filestream);
            }
            world.map_data = map_data;
        }
    });
    player.playername =  read_encoded_string(filestream);
    player.player_id = buffer_read<uint64_t>(filestream);
    player.start_seed =  read_encoded_string(filestream);
    bool has_player_data = buffer_read<bool>(filestream);
    if (has_player_data){
        //playerdata is serialized in PlayerProfile::SavePlayerData
        int32_t player_data_size = buffer_read<int32_t>(filestream);
        player.player_data.resize(player_data_size);
        filestream.read(reinterpret_cast<char *>(player.player_data.data()), player_data_size);
    }
    return player;
}

bool valheim::Player::save_to_file(valheim::PlayerData &player, std::filesystem::path filepath) {

    std::ofstream filestream(filepath, std::ios::binary | std::ios::ate);

    uint32_t header_size = 0;
    //header size will be filled in last
    buffer_write<uint32_t>(filestream, header_size);
    buffer_write<uint32_t>(filestream, player.file_version);
    buffer_write<int32_t>(filestream, player.kills);
    buffer_write<int32_t>(filestream, player.deaths);
    buffer_write<int32_t>(filestream, player.crafts);
    buffer_write<int32_t>(filestream, player.builds);
    buffer_write<int32_t>(filestream, player.worlds.size());
    std::for_each(player.worlds.begin(), player.worlds.end(), [&](PlayerWorldData& world){
        buffer_write<uint64_t>(filestream, world.key);
        buffer_write<bool>(filestream, world.has_custom_spwan_point);
        buffer_write<Position>(filestream, world.custom_spawn_point);
        buffer_write<bool>(filestream, world.has_logout_point);
        buffer_write<Position>(filestream, world.logout_point);
        if (player.file_version >= 30){
            buffer_write<bool>(filestream, world.has_death_point);
            buffer_write<Position>(filestream, world.death_point);
        }
        buffer_write<Position>(filestream, world.home_point);
        buffer_write<bool>(filestream, world.map_data.has_value());
        if (world.map_data.has_value()){
            //write size placeholder here and come back later
            auto buffer_pos = filestream.tellp();
            buffer_write<int32_t>(filestream, 0);
            buffer_write<int32_t>(filestream, world.map_data->map_version);
            buffer_write<int32_t>(filestream, world.map_data->texture_size);
            filestream.write(reinterpret_cast<char *>(world.map_data->explored.data()), world.map_data->explored.size());
            if (world.map_data->map_version >= 2){
            buffer_write<int32_t>(filestream, world.map_data->pins.size());
                for (const auto& pin : world.map_data->pins){
                    write_encoded_string(filestream, pin.name);
                    buffer_write<Position>(filestream, pin.pos);
                    buffer_write<int32_t>(filestream, pin.pin_type);
                    buffer_write<bool>(filestream, pin.checked);
                }
            }
            if (world.map_data->map_version >= 4) {
                buffer_write<bool>(filestream, world.map_data->pos_public);
            }
            auto buffer_after_write_pos = filestream.tellp();
            filestream.seekp(buffer_pos);
            buffer_write<int32_t>(filestream, static_cast<int32_t>(buffer_after_write_pos - buffer_pos - 4));
            filestream.seekp(buffer_after_write_pos);
        }
    });

    write_encoded_string(filestream, player.playername);
    buffer_write<uint64_t>(filestream, player.player_id);
    write_encoded_string(filestream, player.start_seed);

    buffer_write<bool>(filestream, (!player.player_data.empty()));
    if (!player.player_data.empty()){
        buffer_write<int32_t>(filestream, player.player_data.size());
        filestream.write(reinterpret_cast<char *>(player.player_data.data()), player.player_data.size());
    }
    //add dummy sha512 hash, this is not checked so we add 00 bytes
    buffer_write<int32_t>(filestream, 0x40);
    //64 bytes dummy
    for (int i = 0; i < 8; ++i) {
        buffer_write<int64_t>(filestream, 0x00);
    }

    //write file size to header
    auto fileend = filestream.tellp();
    filestream.seekp(0, std::ios::beg);
    //size minus hash size, hash and actual size at start
    buffer_write<int32_t>(filestream, (static_cast<uint32_t >(fileend) - 0x48));

    return true;
}

float valheim::PlayerMapData::calculate_explored_percentage() const {
    size_t ones = 0;
    for (auto& b : explored){
        if (b){
            ++ones;
        }
    }
    //since we can never reach the corners of the map (the map is a circle, the exploration is a square)
    //we calculate the amount of points within the circle as max instead of taking the array size
    //thanks lars for the exact value
    return (static_cast<float>(ones) / 3294198 * 100);
}
