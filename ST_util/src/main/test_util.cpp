/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <fstream>
#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void initialize_SDL(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_WEBP) < 0){
        fprintf(stderr, "Failed to initialize SDL_IMG: %s\n", IMG_GetError());
        exit(1);
    }
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize Audio: %s\n", SDL_GetError());
        exit(1);
    }
    if(TTF_Init() < 0){
        fprintf(stderr, "Failed to initialize SDL_TTF: %s\n", TTF_GetError());
        exit(1);
    }
    Mix_Init(MIX_INIT_OGG);
    if(Mix_OpenAudio(22050,AUDIO_S16SYS,2,640) < 0){
        fprintf(stderr, "Failiure to initialize audio\n");
        exit(1);
    }
}

void close_SDL(){
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool compare_surfaces(SDL_Surface* expected, SDL_Surface* result) {
    if(expected->flags != result->flags){
        return false;
    }
    if(expected->w != result->w){
        return false;
    }
    if(expected->h != result->h){
        return false;
    }
    if(expected->pitch != result->pitch){
        return false;
    }
    auto temp1 = static_cast<char*>(expected->pixels);
    auto temp2 = static_cast<char*>(result->pixels);
    for(int i = 0; i < expected->h*expected->w; i++){
        if(temp1[i] != temp2[i]){
            return false;
        }
    }
    return true;
}

long get_file_size(const std::string& path){
    FILE *pFile = nullptr;
    pFile = fopen(path.c_str(), "rb" );
    fseek( pFile, 0, SEEK_END );
    long size = ftell( pFile );
    fclose( pFile );
    return size;
}

int8_t copy_file(const std::string& src, const std::string& dest){
    FILE* src_file = fopen(src.c_str(), "rb");
    FILE* dest_file = fopen(dest.c_str(), "wb");

    uint32_t size = 16384;
    char buffer[size];

    while(!feof(src_file)){
        size_t n = fread(buffer, 1, size, src_file);
        fwrite(buffer, 1, n, dest_file);
    }

    fflush(dest_file);
    fclose(src_file);
    fclose(dest_file);
}