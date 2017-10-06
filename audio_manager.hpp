#ifndef SOUND_DEF
#define SOUND_DEF

#include "defs.hpp"
#include <unordered_map>



class audio_manager{
    private:
        int volume;
    public:
        audio_manager();
        int initialize();
        void play_music(std::string);
        void play_sound(std::string);
        void stop_music(void);
        int get_volume(void);
        void mute();
        void unmute();
        void close(void);
};

extern audio_manager gAudio_manager;

#endif
