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
audio_manager::audio_manager(task_manager &gTask_manager, message_bus &gMessageBus) : gMessage_bus(gMessageBus), gTask_manager(gTask_manager){
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
    Mix_Volume(-1, chunk_volume);
    Mix_VolumeMusic(music_volume);
    Mix_AllocateChannels(8);

    //subscribe to messages
    gMessage_bus.subscribe(PLAY_SOUND, &msg_sub);
    gMessage_bus.subscribe(PLAY_MUSIC, &msg_sub);
    gMessage_bus.subscribe(STOP_MUSIC, &msg_sub);
    gMessage_bus.subscribe(SET_AUDIO_ENABLED, &msg_sub);
    gMessage_bus.subscribe(MUSIC_ASSETS, &msg_sub);
    gMessage_bus.subscribe(CHUNKS_ASSETS, &msg_sub);
	gMessage_bus.subscribe(STOP_ALL_SOUNDS, &msg_sub);
    gMessage_bus.subscribe(SET_MUSIC_VOLUME, &msg_sub);
    gMessage_bus.subscribe(SET_SOUNDS_VOLUME, &msg_sub);
    gMessage_bus.subscribe(PAUSE_MUSIC, &msg_sub);
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void audio_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        switch (temp->msg_name) {
            case PLAY_SOUND: {
                auto data = temp->base_data0;
                uint16_t name = data & 0x0000ffffU;
                uint8_t volume = (data >> 16U) & 0x000000ffU;
                int8_t loops = (data >> 24U) & 0x000000ffU;
                play_sound(name, volume, loops);
                break;
            }
            case PLAY_MUSIC: {
                auto data = temp->base_data0;
                uint16_t name = data & 0x0000ffffU;
                uint8_t volume = (data >> 16U) & 0x000000ffU;
                int8_t loops = (data >> 24U) & 0x000000ffU;
                play_music(name, volume, loops);
                break;
            }
            case STOP_MUSIC: {
                stop_music();
                gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Music stopped")));
                break;
            }
            case PAUSE_MUSIC: {
                pause_music();
                gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Music paused")));
                break;
            }
            case STOP_ALL_SOUNDS: {
                stop_channels();
                gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Sounds stopped")));
                break;
            }
            case SET_AUDIO_ENABLED: {
                auto arg = static_cast<bool>(temp->base_data0);
                if(!arg){
                    gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Audio muted")));
                    mute();
                }
                else{
                    unmute();
                    gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Audio unmuted")));
                }
                gMessage_bus.send_msg(new message(AUDIO_ENABLED, arg));
                break;
            }
            case MUSIC_ASSETS:
                music_ptr = *static_cast<ska::bytell_hash_map<uint16_t, Mix_Music*>**>(temp->get_data());
                break;
            case CHUNKS_ASSETS:
                chunks_ptr = *static_cast<ska::bytell_hash_map<uint16_t, Mix_Chunk*>**>(temp->get_data());
                break;
            case SET_SOUNDS_VOLUME: {
                set_chunk_volume(static_cast<uint8_t>(temp->base_data0));
                if(!muted) {
                    gMessage_bus.send_msg(new message(SOUNDS_VOLUME_LEVEL, chunk_volume));
                }
                break;
            }
            case SET_MUSIC_VOLUME: {
                set_music_volume(static_cast<uint8_t>(temp->base_data0));
                if (!muted) {
                    gMessage_bus.send_msg(new message(MUSIC_VOLUME_LEVEL, music_volume));
                }
                break;
            }
        }
        delete temp;
        temp = msg_sub.get_next_message();
    }
}

/**
 * Starts the update_task() method using the task manager.
 */
void audio_manager::update(){
    gTask_manager.start_task_lockfree(new ST::task(update_task, this, nullptr));
}


/**
 * Sets the current playback volume for chunks.
 * @param arg the volume level - valid from 0 to 128.
 */
void audio_manager::set_chunk_volume(uint8_t arg) {
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
void audio_manager::set_music_volume(uint8_t arg) {
    music_volume = arg;
    music_playback_volume_ratio = static_cast<float>(MIX_MAX_VOLUME)/ static_cast<float>(music_volume);
    if(!muted){
        Mix_VolumeMusic(music_volume);
    }
}

/**
 * Mutes all audio channels.
 */
void audio_manager::mute(){
    Mix_Volume(-1, 0);
    Mix_VolumeMusic(0);
    muted = true;
}

/**
 * Unmutes all audio channels.
 */
void audio_manager::unmute(){
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
void audio_manager::play_sound(uint16_t arg, uint8_t volume, int8_t loops) const{
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
void audio_manager::play_music(uint16_t arg, uint8_t volume, int8_t loops) const{
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
void audio_manager::stop_music() {
    Mix_HaltMusic();
}

/**
 * Stop the music that is currently playing.
 */
void audio_manager::pause_music() {
    Mix_PauseMusic();
}

/**
 * Stop all sound channels (except for music)
 */
void audio_manager::stop_channels() {
    Mix_HaltChannel(-1);
}

/**
 * Closes the audio subsystem.
 */
audio_manager::~audio_manager(){
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_Quit();
    singleton_initialized = false;
}