#ifndef AUDIO_MNGR_DEF
#define AUDIO_MNGR_DEF

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <assets_manager/assets.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>

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

#endif
