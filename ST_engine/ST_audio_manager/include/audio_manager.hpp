/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef AUDIO_MNGR_DEF
#define AUDIO_MNGR_DEF

#include <SDL.h>
#include <SDL_mixer.h>
#include <message_bus.hpp>
#include <task_manager.hpp>

///This object is responsible for playing sounds and music
class audio_manager{
    friend class audio_manager_test;
    private:

        uint8_t chunk_volume = MIX_MAX_VOLUME;
        uint8_t music_volume = MIX_MAX_VOLUME;
        bool muted = false;
        //ratio: MIX_MAX_VOLUME/volume
        float chunk_playback_volume_ratio = 1;
        float music_playback_volume_ratio = 1;

        ///subscriber object to receive messages
        subscriber msg_sub{};

        ///External dependencies
        ska::bytell_hash_map<uint16_t, Mix_Chunk*>* chunks_ptr{};
        ska::bytell_hash_map<uint16_t, Mix_Music*>* music_ptr{};//-Delivered as a message
        message_bus& gMessage_bus; //-Delivered in constructor
        task_manager& gTask_manager; //-Delivered in constructor

        ///internal update functions
        void handle_messages();


        ///Audio control functions
        void play_music(uint16_t arg, uint8_t volume, int8_t loops) const;
        void play_sound(uint16_t arg, uint8_t volume, int8_t loops) const;
        void mute();
        void unmute();
        static void stop_music() ;
        static void pause_music() ;
        static void stop_channels() ;
        void set_chunk_volume(uint8_t arg);
        void set_music_volume(uint8_t arg);
        static void update_task(void* arg);

    public:
        audio_manager(task_manager &tsk_mngr, message_bus &gMessageBus);
        ~audio_manager();
        void update();
};

//INLINED METHODS

/**
 * Starts the update_task() method using the task manager.
 */
inline void audio_manager::update(){
    gTask_manager.start_task_lockfree(new ST::task(update_task, this, nullptr));
}


/**
 * Sets the current playback volume for chunks.
 * @param arg the volume level - valid from 0 to 128.
 */
inline void audio_manager::set_chunk_volume(uint8_t arg) {
    chunk_volume = arg;
    chunk_playback_volume_ratio = static_cast<float>(MIX_MAX_VOLUME)/ static_cast<float>(chunk_volume);
    if(!muted){
        Mix_Volume(-1, chunk_volume);
    }
}

/**
 * Sets the current playback volume for music.
 * @param arg the volume level - valid from 0 to 128.
 */
inline void audio_manager::set_music_volume(uint8_t arg) {
    music_volume = arg;
    music_playback_volume_ratio = static_cast<float>(MIX_MAX_VOLUME)/ static_cast<float>(music_volume);
    if(!muted){
        Mix_VolumeMusic(music_volume);
    }
}

/**
 * Mutes all audio channels.
 */
inline void audio_manager::mute(){
    Mix_Volume(-1, 0);
    Mix_VolumeMusic(0);
    muted = true;
}

/**
 * Unmutes all audio channels.
 */
inline void audio_manager::unmute(){
    Mix_Volume(-1, chunk_volume);
    Mix_VolumeMusic(music_volume);
    muted = false;
}

/**
 * Plays a sound.
 * @param arg The sound to play - a hashed value of the original filename.
 * @param volume The volume to play the chunk at.
 * @param loops How many times to play it.
 */
inline void audio_manager::play_sound(uint16_t arg, uint8_t volume, int8_t loops) const{
    auto data = chunks_ptr->find(arg);
    auto chunk = reinterpret_cast<Mix_Chunk*>((data != chunks_ptr->end())*reinterpret_cast<uint64_t>(data->second));
    if(!muted && chunk){ //null-check can be removed with next release of SDL_Mixer
        Mix_VolumeChunk(chunk, static_cast<int>(static_cast<float>(volume) / chunk_playback_volume_ratio));
    }
    if(Mix_PlayChannel( -1, chunk, loops ) == -1){
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("Mix_PlayChannel Error " + std::string(Mix_GetError()))));
    }
}

/**
 *
 * @param arg The music to play - a hashed value of the original filename.
 * @param volume The volume to play the music at.
 * @param loops How many times to play it, -1 will loop indefinitely.
 */
inline void audio_manager::play_music(uint16_t arg, uint8_t volume, int8_t loops) const{
    auto data = music_ptr->find(arg);
    auto music = reinterpret_cast<Mix_Music*>((data != music_ptr->end())*reinterpret_cast<uint64_t>(data->second));
    if(!muted) {
        Mix_VolumeMusic(static_cast<int>(static_cast<float>(volume) / music_playback_volume_ratio));
    }
    if(Mix_PlayMusic(music, loops) == -1){
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("Mix_PlayMusic Error " + std::string(Mix_GetError()))));
    }
}

/**
 * Stop the music that is currently playing.
 */
inline void audio_manager::stop_music() {
    Mix_HaltMusic();
}

/**
 * Stop the music that is currently playing.
 */
inline void audio_manager::pause_music() {
    Mix_PauseMusic();
}

/**
 * Stop all sound channels (except for music)
 */
inline void audio_manager::stop_channels() {
    Mix_HaltChannel(-1);
}

#endif
