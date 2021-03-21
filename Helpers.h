//
// Created by lpcvoid on 21/03/2021.
//

#pragma once

int32_t read_7bit_enc_int(std::ifstream& filestream){
    int32_t res = 0;
    int32_t shifter = 0;
    uint8_t mask = 0;
    do {
        if (shifter == 35){
            return 0;
        }
        filestream.read(reinterpret_cast<char *>(&mask), 1);
        res |= (mask & 0x7f) << shifter;
        shifter += 7;
    } while ((mask & 0x80));
    return res;
}

std::string read_encoded_string(std::ifstream& filestream){
    std::string res;
    int32_t sz = 0;
    int32_t len = read_7bit_enc_int(filestream);
    if (!len)
        return res;
    char buffer[128];
    do {
        int32_t count = (len - sz > 128) ? 128 : (len - sz);
        filestream.read(reinterpret_cast<char *>(&buffer[0]), count);
        if ((!sz) && (count == len))
            return std::string(buffer, count);
        sz += count;
    } while (sz < len);

}

