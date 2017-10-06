#include "audio_manager.hpp"
#include "level_manager.hpp"
#include "assets_manager.hpp"

audio_manager::audio_manager(){
    //empty constructor
}

int audio_manager::get_volume(){
    return volume;
}

void audio_manager::mute(){
    Mix_Volume(-1, 0);
    Mix_VolumeMusic(0);
    volume = 0;
}

void audio_manager::unmute(){
    Mix_Volume(-1, MIX_MAX_VOLUME);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    volume = MIX_MAX_VOLUME;
}

void audio_manager::close(void){
    Mix_CloseAudio();
    Mix_Quit();
}

void audio_manager::play_sound(std::string arg){
    Mix_Chunk* temp = gAssets_manager.get_assets()->sounds.chunks[arg];
    if(Mix_PlayChannel( 0, temp, 0 ) == -1)
        printf("Mix_PlayChannel: %s\n", Mix_GetError());
}

void audio_manager::play_music(std::string arg){
    Mix_Music* temp = gAssets_manager.get_assets()->sounds.music[arg];
    if(Mix_PlayMusic(temp, -1 )==-1)
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
}

void audio_manager::stop_music(){
    Mix_PauseMusic();
}

int audio_manager::initialize(){
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL_MIXER: " << SDL_GetError() << "\n";
        exit(1);
    }
    if(Mix_OpenAudio(22050,AUDIO_S16SYS,2,640) == -1){
        printf("Failiure to initialize audio\n");
		return -1;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    volume = MIX_MAX_VOLUME;
    return 0;
}
