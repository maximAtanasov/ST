/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <audio_manager/audio_manager.hpp>

/**
 * Performs the update for the asset_manager on a task thread.
 * @param arg pointer to an audio_manager (a <b>this</b> pointer basically) as the
 * function must be static.
 */
void audio_manager::update_task(void* arg){
    auto mngr = static_cast<audio_manager*>(arg);
    mngr->handle_messages();
}

/**
 * Initializes the audio subsystem.
 * Allocates audio channels.
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task manager.
 */
audio_manager::audio_manager(message_bus* msg_bus, task_manager* tsk_mngr){
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL_MIXER: %s\n", SDL_GetError());
        exit(1);
    }
    Mix_Init(MIX_INIT_OGG);
    if(Mix_OpenAudio(22050,AUDIO_S16SYS,2,640) == -1){
        fprintf(stderr, "Failiure to initialize audio\n");
        exit(1);
    }
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_AllocateChannels(16);

    //subscribe to messages
    gMessage_bus->subscribe(PLAY_SOUND, &msg_sub);
    gMessage_bus->subscribe(PLAY_MUSIC, &msg_sub);
    gMessage_bus->subscribe(STOP_MUSIC, &msg_sub);
    gMessage_bus->subscribe(TOGGLE_AUDIO, &msg_sub);
    gMessage_bus->subscribe(ASSETS, &msg_sub);
	gMessage_bus->subscribe(STOP_ALL_SOUNDS, &msg_sub);
    gMessage_bus->subscribe(SET_VOLUME, &msg_sub);
    volume = MIX_MAX_VOLUME;
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void audio_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == PLAY_SOUND){
            auto data = static_cast<std::tuple<size_t, uint8_t, int8_t>*>(temp->get_data());
            size_t name = std::get<0> (*data);
            uint8_t volume = std::get<1> (*data);
            int8_t loops = std::get<2> (*data);
            play_sound(name, volume, loops);
        }
        else if(temp->msg_name == PLAY_MUSIC){
            auto data = (std::tuple<size_t, uint8_t, int8_t>*)temp->get_data();
            size_t name = std::get<0> (*data);
            uint8_t volume = std::get<1> (*data);
            int8_t loops = std::get<2> (*data);
            play_music(name, volume, loops);
        }
        else if(temp->msg_name == STOP_MUSIC){
            stop_music();
            log(SUCCESS, "Music stopped");
        }
        else if(temp->msg_name == STOP_ALL_SOUNDS){
			stop_channels();
            log(SUCCESS, "Sounds stopped");
        }
        else if(temp->msg_name == TOGGLE_AUDIO){
            if(volume > 0){
                log(SUCCESS, "Audio OFF");
                mute();
                 set_volume(0);
            }
            else{
                set_volume(MIX_MAX_VOLUME);
                unmute();
                log(SUCCESS, "Audio ON");
            }
            gMessage_bus->send_msg(make_msg(VOLUME_LEVEL, make_data(volume)));
        }
        else if(temp->msg_name == ASSETS){
            auto temp_ptr = static_cast<ST::assets**>(temp->get_data());
            assets_ptr = *temp_ptr;
        }
        else if(temp->msg_name == SET_VOLUME){
            auto arg = static_cast<uint8_t*>(temp->get_data());
            set_volume(*arg);
            gMessage_bus->send_msg(make_msg(VOLUME_LEVEL, make_data(volume)));
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}

/**
 * Consumes any leftover messages and closes the audio subsystem.
 */
audio_manager::~audio_manager(){
    handle_messages();
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_Quit();
}