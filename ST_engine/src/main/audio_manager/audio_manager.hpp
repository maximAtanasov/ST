/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef AUDIO_MNGR_DEF
#define AUDIO_MNGR_DEF

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <assets_manager/assets.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>
#include <console/log.hpp>

class audio_manager{
    private:

        int volume = MIX_MAX_VOLUME;

        //subscriber object to receive messages
        subscriber* msg_sub{};

        //External dependencies
        ST::assets* assets_ptr{}; //-Delivered as a message
        message_bus* gMessage_bus{}; //-Delivered in constructor
        task_manager* gTask_manager{}; //-Delivered in constructor

        //internal update functions
        void handle_messages();


        //Audio control functions
        void play_music(size_t arg, int volume, int loops);
        void play_sound(size_t arg, int volume, int loops);
        void mute();
        void unmute();
        void stop_music();
        void stop_channels();
        void set_volume(Uint8 arg);
        static void update_task(void* arg);

    public:
        audio_manager() = default;
        void update();
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void close();

};

//INLINED METHODS

inline void audio_manager::update(){
        //start a task that will handle messages (play audio and such)
        gTask_manager->start_task_lockfree(new ST::task(update_task, this, nullptr, -1));
}

inline void audio_manager::set_volume(Uint8 arg) {
        volume = arg;
}

inline void audio_manager::mute(){
        Mix_Volume(-1, 0);
        Mix_VolumeMusic(0);
        volume = 0;
}

inline void audio_manager::unmute(){
        Mix_Volume(-1, MIX_MAX_VOLUME);
        Mix_VolumeMusic(MIX_MAX_VOLUME);
        volume = MIX_MAX_VOLUME;
}

inline void audio_manager::close(){
        handle_messages();
        delete msg_sub;
        Mix_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        Mix_Quit();
}

inline void audio_manager::play_sound(size_t arg, int volume, int loops){
        Mix_Chunk* data = static_cast<Mix_Chunk*>(assets_ptr->chunks->get(arg));
        if(data != nullptr){
            Mix_VolumeChunk(data, volume);
            if(Mix_PlayChannel( -1, data, loops ) == -1){
                log(ERROR, "Mix_PlayChannel Error " + std::string(Mix_GetError()));
            }
        }
}

inline void audio_manager::play_music(size_t arg, int volume, int loops){
        Mix_Music* data = assets_ptr->music[arg];
        if(data != nullptr){
                Mix_VolumeMusic(volume);
                if(Mix_PlayMusic(data, loops) == -1){
                        log(ERROR, "Mix_PlayMusic Error " + std::string(Mix_GetError()));
                }
                Mix_VolumeMusic(this->volume);
        }
}

inline void audio_manager::stop_music(){
        Mix_PauseMusic();
}

inline void audio_manager::stop_channels(){
        Mix_HaltChannel(-1);
}

#endif
