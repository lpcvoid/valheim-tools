//
// Created by lpcvoid on 03/04/2021.
//

#pragma once

#include <cstdint>
#include <utility>
#include <map>
#include "Player.h"

namespace valheim {



    struct ZDOData {
        uint32_t owner_rev = 0;
        uint32_t data_rev = 0;
        bool persistent = false;
        uint64_t owner = 0;
        uint64_t time_created = 0;
        int32_t pgw_version = 0;
        int8_t zdo_type = 0;
        bool distant = false;
        int32_t prefab = 0;
        Vec2<int32_t>  sector;
        Position position;
        Quaternion rotation;
        std::map<int32_t, float> floats;
        std::map<int32_t , Vec3<float>> coordinates;
        std::map<int32_t , Quaternion> rotations;
        std::map<int32_t , int32_t> ints;
        std::map<int32_t , int64_t> longs;
        std::map<int32_t , std::string> strings;
    };

    struct ZDOID {
        uint64_t  user_id = 0;
        uint32_t  id = 0;
        ZDOData data;
    };

    struct WorldData {
        std::string name;
        std::string seed_name;
        int32_t seed = 0;
        int64_t uid = 0;
        int32_t world_gen_version = 0;
        uint64_t net_time = 0;
        std::vector<ZDOID> zdos;
    };


    class World {
    private:
        std::vector<ZDOID> _zdos;
    public:
        [[nodiscard]] std::optional<WorldData> load_from_file(std::filesystem::path filepath);
    };

}


