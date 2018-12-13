/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#ifdef _MSC_VER
#include <Windows.h>
#endif

void initialize_SDL(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_WEBP) < 0){
        fprintf(stderr, "Failed to initialize SDL_IMG: %s\n", IMG_GetError());
        exit(1);
    }
    if(TTF_Init() < 0){
        fprintf(stderr, "Failed to initialize SDL_TTF: %s\n", TTF_GetError());
        exit(1);
    }
    if(Mix_OpenAudio(22050, AUDIO_F32SYS ,2, 1024) < 0){
        fprintf(stderr, "Failiure to initialize audio\n");
        exit(1);
    }
    if(Mix_Init(MIX_INIT_OGG) < 0){
        fprintf(stderr, "Failed to initialize SDL_MIXER: %s\n", Mix_GetError());
        exit(1);
    }
}

void close_SDL(){
    while(Mix_Init(0)) {
        Mix_Quit();
    }
    Mix_CloseAudio();
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

void copy_file(const std::string& src, const std::string& dest){
#ifndef _MSC_VER
    std::ifstream  src_(src, std::ios::binary);
    std::ofstream  dst_(dest, std::ios::binary);
    dst_ << src_.rdbuf();
#else
    CopyFile(src.c_str(), dest.c_str(), false);
#endif
}