#ifndef DM_DEF
#define DM_DEF

//SINGLETON CLASS

#include "defs.hpp"

class display_manager{
    
    private:
        SDL_Window* window;
        SDL_DisplayMode DM;
        int height; 
        int width; 

    public:
        display_manager();
        int initialize(void);
        void set_fullscreen(void);
        void set_windowed(void);
        SDL_Window* get_window(void);
        void close(void);
};

extern display_manager gDisplay_manager;

#endif