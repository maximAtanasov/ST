/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <ST_loaders/loaders.hpp>
#include <ST_util/test_util.hpp>

/**
 * \mainpage A small program that will combine multiple png, wav and ogg files into a single file that can
 * then be read by the slavicTales game engine.
 * File format version: v0.9
 * Maxim Atanasov
 */

 //When testing the program, we only test if the proper arguments run the proper blocks of code. We do not
 //Generate actual binaries - that is already tested in ST_loaders library.
#ifndef TESTING
int main(int argc, char *argv[]) {
#elif defined(TESTING)
int asset_pack_main(int argc, char *argv[]) {
#endif
#ifndef TESTING
     initialize_SDL();
#endif
     
     if (argc < 3) {
         fprintf(stderr, "Not enough arguments!\n");
         return -1;
     }

     std::string pack_arg = std::string(argv[1]);

     //Copy the char** to a nice little std::vector of std::string's
     std::vector<std::string> args;
     for (int i = 2; i < argc; i++) {
         args.emplace_back(std::string(argv[i]));
     }

     if (pack_arg == "-p" || pack_arg == "--pack") { //Pack assets to a binary
         std::string binary_name = args.at(0);
         args.erase(args.begin(), args.begin() + 1);
         #ifndef TESTING
         int8_t return_code = ST::pack_to_binary(binary_name, args);
         if(return_code == -1){
            fprintf(stderr, "Error packing files to existing binary, maybe it is corrupted!\n");
         }
         else if(return_code == -2){
             fprintf(stderr, "Error packing files to existing binary, it already cointains a file named the same as one of the ones you are adding!\n");
         }
         else if(return_code == 0)
         #endif
            fprintf(stdout, "Binary generated!\n");
     }
     else if (pack_arg == "-u" || pack_arg == "--unpack") { //Or unpack a binary to disk
         for (const auto &path : args) {
            #ifndef TESTING
            if(ST::unpack_binary_to_disk(path) == 0){
                fprintf(stdout, "Binary unpacked!\n");
            }
            else{
                fprintf(stderr, "Error unpacking!\n");
            }
            #elif defined(TESTING)
            fprintf(stdout, "Binary unpacked!\n");
            #endif
         }
     }
     else {
         fprintf(stderr, "Unknown argument!\n");
     }
#ifndef TESTING
    close_SDL();
#endif
     return 0;
 }