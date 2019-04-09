/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <audio_manager.hpp>

static bool singleton_initialized = false;

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
    if(singleton_initialized){
        throw std::runtime_error("The audio manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL audio subsystem: %s\n", SDL_GetError());
        exit(1);
    }
    if(Mix_OpenAudio(22050, AUDIO_F32SYS ,2, 640) == -1){
        fprintf(stderr, "Failiure to open audio device\n");
        exit(1);
    }
	if (Mix_Init(MIX_INIT_OGG) == 0) {
		fprintf(stderr, "Failed to initialize SDL_Mixer: %s\n", Mix_GetError());
		exit(1);
	}
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    Mix_Volume(-1, chunk_volume);
    Mix_VolumeMusic(music_volume);
    Mix_AllocateChannels(8);

    //subscribe to messages
    gMessage_bus->subscribe(PLAY_SOUND, &msg_sub);
    gMessage_bus->subscribe(PLAY_MUSIC, &msg_sub);
    gMessage_bus->subscribe(STOP_MUSIC, &msg_sub);
    gMessage_bus->subscribe(SET_AUDIO_ENABLED, &msg_sub);
    gMessage_bus->subscribe(MUSIC_ASSETS, &msg_sub);
    gMessage_bus->subscribe(CHUNKS_ASSETS, &msg_sub);
	gMessage_bus->subscribe(STOP_ALL_SOUNDS, &msg_sub);
    gMessage_bus->subscribe(SET_MUSIC_VOLUME, &msg_sub);
    gMessage_bus->subscribe(SET_SOUNDS_VOLUME, &msg_sub);
    gMessage_bus->subscribe(PAUSE_MUSIC, &msg_sub);
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void audio_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == PLAY_SOUND){
            auto data = static_cast<std::tuple<uint16_t, uint8_t, int8_t>*>(temp->get_data());
            uint16_t name = std::get<0> (*data);
            uint8_t volume = std::get<1> (*data);
            int8_t loops = std::get<2> (*data);
            play_sound(name, volume, loops);
        }
        else if(temp->msg_name == PLAY_MUSIC){
            auto data = static_cast<std::tuple<uint16_t, uint8_t, int8_t>*>(temp->get_data());
            uint16_t name = std::get<0> (*data);
            uint8_t volume = std::get<1> (*data);
            int8_t loops = std::get<2> (*data);
            play_music(name, volume, loops);
        }
        else if(temp->msg_name == STOP_MUSIC){
            stop_music();
            gMessage_bus->send_msg(new message(LOG_SUCCESS, make_data<std::string>("Music stopped")));
        }
        else if(temp->msg_name == PAUSE_MUSIC){
            pause_music();
            gMessage_bus->send_msg(new message(LOG_SUCCESS, make_data<std::string>("Music paused")));
        }
        else if(temp->msg_name == STOP_ALL_SOUNDS){
			stop_channels();
            gMessage_bus->send_msg(new message(LOG_SUCCESS, make_data<std::string>("Sounds stopped")));
        }
        else if(temp->msg_name == SET_AUDIO_ENABLED){
            auto arg = static_cast<bool*>(temp->get_data());
            if(!*arg){
                gMessage_bus->send_msg(new message(LOG_SUCCESS, make_data<std::string>("Audio muted")));
                mute();
            }
            else{
                unmute();
                gMessage_bus->send_msg(new message(LOG_SUCCESS, make_data<std::string>("Audio unmuted")));
            }
            gMessage_bus->send_msg(new message(AUDIO_ENABLED, make_data(*arg)));
        }
        else if(temp->msg_name == MUSIC_ASSETS){
            music_ptr = *static_cast<ska::bytell_hash_map<uint16_t, Mix_Music*>**>(temp->get_data());
        }
        else if(temp->msg_name == CHUNKS_ASSETS){
            chunks_ptr = *static_cast<ska::bytell_hash_map<uint16_t, Mix_Chunk*>**>(temp->get_data());
        }
        else if(temp->msg_name == SET_SOUNDS_VOLUME){
            auto arg = static_cast<uint8_t*>(temp->get_data());
            set_chunk_volume(*arg);
            if(!muted) {
                gMessage_bus->send_msg(new message(SOUNDS_VOLUME_LEVEL, make_data(chunk_volume)));
            }
        }
        else if(temp->msg_name == SET_MUSIC_VOLUME){
            auto arg = static_cast<uint8_t*>(temp->get_data());
            set_music_volume(*arg);
            if(!muted) {
                gMessage_bus->send_msg(new message(MUSIC_VOLUME_LEVEL, make_data(music_volume)));
            }
        }
        delete temp;
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
    singleton_initialized = false;
}