#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <ST_loaders/loaders.hpp>

/*
 * A small program that will combine multiple png images into a single file that can
 * then be read by the slavicTales game engine.
 * File format version: v0.1
 * Maxim Atanasov
 */

void initialize_SDL();
void close_SDL();

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

void initialize_SDL(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if(IMG_Init(IMG_INIT_PNG) < 0){
        fprintf(stderr, "Failed to initialize SDL_IMG: %s\n", IMG_GetError());
        exit(1);
    }

    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL_MIXER: %s\n", SDL_GetError());
        exit(1);
    }
    Mix_Init(MIX_INIT_OGG);
    if(Mix_OpenAudio(22050,AUDIO_S16SYS,2,640) == -1){
        fprintf(stderr, "Failiure to initialize audio\n");
        exit(1);
    }
}

void close_SDL(){
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}