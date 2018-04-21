/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <ST_loaders/loaders.hpp>
#include <ST_util/test_util.hpp>

/*
 * A small program that will combine multiple png, wav and ogg files into a single file that can
 * then be read by the slavicTales game engine.
 * File format version: v0.5
 * Maxim Atanasov
 */

int main(int argc, char* argv[]) {
    initialize_SDL();
    if(argc < 3){
        fprintf(stderr, "Not enough arguments!\n");
        exit(1);
    }
    std::string pack_arg = std::string(argv[1]);
    //Copy the char** to a nice little std::vector of std::string's
    std::vector<std::string> args;
    for(int i = 2; i < argc; i++){
        args.emplace_back(std::string(argv[i]));
    }
    if(pack_arg == "-p") {
        std::string binary_name = args.at(0);
        args.erase(args.begin(), args.begin()+1);
        ST::pack_to_binary(binary_name,  args);
    }else if(pack_arg == "-u"){
        for(const auto& path : args){
            ST::unpack_binary_to_disk(path);
        }
    }else{
        fprintf(stderr, "Unknown argument!\n");
    }
    close_SDL();
    return 0;
}