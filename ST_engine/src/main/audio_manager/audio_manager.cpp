/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <audio_manager/audio_manager.hpp>
#include <console/log.hpp>

void audio_manager::update_task(void* arg){
    auto mngr = (audio_manager*)arg;
    mngr->handle_messages();
}

int audio_manager::initialize(message_bus* msg_bus, task_manager* tsk_mngr){
    if(SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL_MIXER: %s\n", SDL_GetError());
        return -1;
    }
    Mix_Init(MIX_INIT_OGG);
    if(Mix_OpenAudio(22050,AUDIO_S16SYS,2,640) == -1){
        fprintf(stderr, "Failiure to initialize audio\n");
		return -1;
    }
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Mix_AllocateChannels(16);

    //create a subscriber and subscribe to messages
    msg_sub = new subscriber();
    gMessage_bus->subscribe(PLAY_SOUND, msg_sub);
    gMessage_bus->subscribe(PLAY_MUSIC, msg_sub);
    gMessage_bus->subscribe(STOP_MUSIC, msg_sub);
    gMessage_bus->subscribe(TOGGLE_AUDIO, msg_sub);
    gMessage_bus->subscribe(ASSETS, msg_sub);
	gMessage_bus->subscribe(STOP_ALL_SOUNDS, msg_sub);
    volume = 100;
    return 0;
}

void audio_manager::handle_messages(){
    message* temp = msg_sub->get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == PLAY_SOUND){
            auto data = static_cast<std::tuple<size_t, int, int>*>(temp->get_data());
            size_t name = std::get<0> (*data);
            int volume = std::get<1> (*data);
            int loops = std::get<2> (*data);
            play_sound(name, volume, loops);
        }
        else if(temp->msg_name == PLAY_MUSIC){
            auto data = (std::tuple<size_t, int, int>*)temp->get_data();
            size_t name = std::get<0> (*data);
            int volume = std::get<1> (*data);
            int loops = std::get<2> (*data);
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
            }
            else{
                unmute();
                log(SUCCESS, "Audio ON");
            }
            gMessage_bus->send_msg(make_msg(VOLUME_LEVEL, make_data<int>(volume)));
        }
        else if(temp->msg_name == ASSETS){
            auto temp_ptr = (ST::assets**)temp->get_data();
            assets_ptr = *temp_ptr;
        }else if(temp->msg_name == SET_VOLUME){
            auto arg = (Uint8*)temp->get_data();
            set_volume(*arg);
        }
        destroy_msg(temp);
        temp = msg_sub->get_next_message();
    }
}
