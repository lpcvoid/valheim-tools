//
// Created by lpcvoid on 03/04/2021.
//

#include <fstream>
#include <iostream>
#include "World.h"
#include "Helpers.h"

#define READ_MAP(DTYPE,MEMBER) \
        filestream.read(reinterpret_cast<char *>(&count), 1);                   \
        for (int i = 0; i < count; i++){                                        \
            int32_t key = 0;                                                    \
            DTYPE value = {};                                                   \
            filestream.read(reinterpret_cast<char *>(&key), 4);                 \
            filestream.read(reinterpret_cast<char *>(&value), sizeof(DTYPE));   \
            zdo.data.MEMBER[key] = value;                                       \
        }                                                                       \

std::optional<valheim::WorldData> valheim::World::load_from_file(std::filesystem::path filepath) {

    std::ifstream filestream(filepath, std::ios::binary | std::ios::ate);

    auto fileend = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
    auto filesize = fileend - filestream.tellg();

    WorldData world;

    filestream.seekg(0, std::ios::beg);
    int32_t file_version = 0;
    filestream.read(reinterpret_cast<char *>(&file_version), 4);

    if (file_version >= 4){
        filestream.read(reinterpret_cast<char *>(&world.net_time), 8);
    }

    filestream.ignore(8);

    uint32_t zdo_largest_id = 0;
    int32_t zdo_count = 0;

    filestream.read(reinterpret_cast<char *>(&zdo_largest_id), 4);
    filestream.read(reinterpret_cast<char *>(&zdo_count), 4);

    world.zdos.resize(zdo_count);

    std::for_each(world.zdos.begin(), world.zdos.end(), [&](ZDOID& zdo){
        filestream.read(reinterpret_cast<char *>(&zdo.user_id), 8);
        filestream.read(reinterpret_cast<char *>(&zdo.id), 4);
        filestream.ignore(4); //following zpackage size which we ignore

        //zdo load
        filestream.read(reinterpret_cast<char *>(&zdo.data.owner_rev), 4);
        filestream.read(reinterpret_cast<char *>(&zdo.data.data_rev), 4);
        filestream.read(reinterpret_cast<char *>(&zdo.data.persistent), 1);
        filestream.read(reinterpret_cast<char *>(&zdo.data.owner), 8);
        filestream.read(reinterpret_cast<char *>(&zdo.data.time_created), 8);
        filestream.read(reinterpret_cast<char *>(&zdo.data.pgw_version), 4);
        if ((file_version >= 16) && (file_version < 24)){
            filestream.ignore(4);
        }
        if (file_version >= 23){
            filestream.read(reinterpret_cast<char *>(&zdo.data.zdo_type), 1);
        }
        if (file_version >= 22){
            filestream.read(reinterpret_cast<char *>(&zdo.data.distant), 1);
        }
        if (file_version < 13){
            filestream.ignore(2);
        }
        if (file_version > 17){
            filestream.read(reinterpret_cast<char *>(&zdo.data.prefab), 4);
        }
        filestream.read(reinterpret_cast<char *>(&zdo.data.sector), sizeof(Vec2<int32_t>));
        filestream.read(reinterpret_cast<char *>(&zdo.data.position), sizeof(Position));
        filestream.read(reinterpret_cast<char *>(&zdo.data.rotation), sizeof(Quaternion));

        uint8_t count = 0;

        READ_MAP(float,floats)
        READ_MAP(Vec3<float>,coordinates)
        READ_MAP(Quaternion ,rotations)
        READ_MAP(int32_t ,ints)
        READ_MAP(uint64_t ,longs)

        filestream.read(reinterpret_cast<char *>(&count), 1);
        for (int i = 0; i < count; i++){
            int32_t key = 0;
            std::string value = {};
            filestream.read(reinterpret_cast<char *>(&key), 4);
            value = read_encoded_string(filestream);
            zdo.data.strings[key] = value;
        }
    });

    return world;

}
