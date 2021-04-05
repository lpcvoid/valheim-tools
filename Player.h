//
// Created by lpcvoid on 21/03/2021.
//

#pragma once


#include <cstdint>
#include <tuple>
#include <vector>
#include <memory>
#include <filesystem>
#include "Types.h"

namespace valheim {

    struct PlayerPinData {
        std::string name;
        Position pos;
        int32_t pin_type = 0;
        bool checked = false;
    };

    using PlayerMapExploreData = std::vector<uint8_t>;

    struct PlayerMapData {
        uint32_t map_version = 0;
        uint32_t texture_size = 0;
        PlayerMapExploreData explored;
        std::vector<PlayerPinData> pins;
        bool pos_public = false;
        float calculate_explored_percentage() const;
    };

    struct PlayerWorldData {
        uint64_t key = 0;
        bool has_custom_spwan_point = false;
        Position custom_spawn_point;
        bool has_logout_point = false;
        Position  logout_point;
        bool has_death_point = false;
        Position death_point;
        Position home_point;
        std::optional<PlayerMapData> map_data;
    };

    struct PlayerData {
        uint32_t file_version = 0;
        int32_t kills = 0;
        int32_t deaths = 0;
        int32_t crafts = 0;
        int32_t builds = 0;
        std::vector<PlayerWorldData> worlds;
        std::string playername;
        uint64_t player_id = 0;
        std::string start_seed;
        std::vector<uint8_t> player_data;
    };

    class Player {
        private:

        public:
            [[nodiscard]] std::optional<PlayerData> load_from_file(std::filesystem::path filepath);
            bool save_to_file(PlayerData& player, std::filesystem::path filepath);

    };

}




