#include <iostream>
#include <filesystem>

void print_help(){

    std::cout << "Usage : " << std::endl;
    std::cout << "playerdb_reader [PATH_TO_PLAYERDB]" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "Valheim player db reader" << std::endl;
    if (1 == argc)
        print_help();
    else if (2 == argc ){
        std::string playerdb_filepath = argv[1];
        if (std::filesystem::exists(playerdb_filepath)){
            //load the file
        } else {
            std::cerr << "File not found!" << std::endl;
        }
    } else {
        std::cerr << "Too many arguments!" << std::endl;
        print_help();
    }
}
