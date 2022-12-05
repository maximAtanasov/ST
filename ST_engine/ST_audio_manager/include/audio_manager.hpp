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
class audio_manager {
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
    ska::bytell_hash_map<uint16_t, Mix_Chunk *> *chunks_ptr{};
    ska::bytell_hash_map<uint16_t, Mix_Music *> *music_ptr{};//-Delivered as a message
    message_bus &gMessage_bus; //-Delivered in constructor
    task_manager &gTask_manager; //-Delivered in constructor

    ///internal update functions
    void handle_messages();


    ///Audio control functions
    void play_music(uint16_t arg, uint8_t volume, int8_t loops) const;

    void play_sound(uint16_t arg, uint8_t volume, int8_t loops) const;

    void mute();

    void unmute();

    static void stop_music();

    static void pause_music();

    static void stop_channels();

    void set_chunk_volume(uint8_t arg);

    void set_music_volume(uint8_t arg);

    static void update_task(void *arg);

public:
    audio_manager(task_manager &tsk_mngr, message_bus &gMessageBus);

    ~audio_manager();

    void update();
};


#endif
