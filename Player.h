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

    struct PlayerWorldData {
        uint64_t key = 0;
        bool has_custom_spwan_point = false;
        Position custom_spawn_point;
        bool has_logout_point = false;
        Position  logout_point;
        bool has_death_point = false;
        Position death_point;
        Position home_point;
        std::vector<uint8_t> map_data;
    };

    struct PlayerData {
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
    };

}




