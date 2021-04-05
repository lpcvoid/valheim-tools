//
// Created by lpcvoid on 21/03/2021.
//

#pragma once

//thank you c++ standards committee for making the whole streams
//class design so clunky that I feel the need to wrap it every time
template <typename T>
T buffer_read(std::ifstream& filestream){
    T result;
    filestream.read(reinterpret_cast<char *>(&result), sizeof(T));
    return result;
}

template <typename T>
void buffer_write(std::ofstream& filestream, T data){
    filestream.write(reinterpret_cast<char *>(&data), sizeof(T));
}


void write_7bit_enc_int(std::ofstream& filestream, int32_t val){
    uint32_t num = val;
    for (num = (uint)val; num >= 128; num >>= 7)
    {
        buffer_write<uint8_t>(filestream, static_cast<uint8_t>((num | 0x80)));
    }
    buffer_write<uint8_t>(filestream, num);
}

void write_encoded_string(std::ofstream& filestream, std::string val){
    write_7bit_enc_int(filestream, val.size());
    filestream.write(reinterpret_cast<char *>(val.data()), val.size());
}

int32_t read_7bit_enc_int(std::ifstream& filestream){
    int32_t res = 0;
    int32_t shifter = 0;
    uint8_t mask = 0;
    do {
        if (shifter == 35){
            return 0;
        }
        mask = buffer_read<uint8_t>(filestream);
        res |= (mask & 0x7f) << shifter;
        shifter += 7;
    } while ((mask & 0x80));
    return res;
}

std::string read_encoded_string(std::ifstream& filestream){
    std::string res;
    int32_t len = read_7bit_enc_int(filestream);
    if (!len)
        return res;
    res.resize(len);
    filestream.read(reinterpret_cast<char *>(res.data()), len);
    return res;
}





