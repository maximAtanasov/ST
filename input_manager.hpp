#ifndef INPUT_DEF
#define INPUT_DEF

#include "defs.hpp"

typedef struct gameControls{
    Uint8* keyboardFramePrev;
    int mouseClicks[3];
    int mouseClicksFramePrev[3];
    const Uint8* keyboard;
    int mouseX, mouseY;
    int mouse_scroll;
} game_controls;

class input_manager{
    private:
        SDL_Event event;
        game_controls controls;
        int keypress(std::string);
        int keyheld(std::string);
        int keyrelease(std::string);
    public:
        input_manager();
        int initialize();
        void close();
        void take_input();
        int get_mouseX();
        int get_mouseY();
        int key_pressed(std::string);
        int key_held(std::string);
        int key_released(std::string);
        inline int mouse_scroll(){
            return controls.mouse_scroll;
        }
};

extern input_manager gInput_manager;

#endif
