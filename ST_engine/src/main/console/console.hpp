#ifndef DEVCONSOLE_DEF
#define DEVCONSOLE_DEF

//SINGLETON CLASS

#include "../defs.hpp"
#include "console_log.hpp"
#include "../message_bus/message_bus.hpp"


class console{
    private:
        void scroll(int scroll_y);
        void toggle();
        void show();
        void hide();
        void write(console_log arg);
        void handle_messages();

        Uint8 log_level = 0x00;


    public:
        SDL_Rect window{};
        SDL_Color color{};
        SDL_Color color_text{};
        subscriber* msg_sub{};
        bool shown = false;
        std::vector<console_log> entries;
        int pos = 0;
        int font_size = 0;
        int scroll_offset = 0;

        //things to do with inputting text commands
        std::string composition;
        std::string* stream{};

        message_bus* gMessage_bus{};

        console() = default;
        int initialize(message_bus* msg_bus);
        void set_log_level(log_type arg);
        bool is_open();
        void update();
        void close();
};

#endif
