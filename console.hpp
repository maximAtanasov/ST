#ifndef DEVCONSOLE_DEF
#define DEVCONSOLE_DEF

//SINGLETON CLASS

#include "defs.hpp"

class console{
    private:
        SDL_Rect window;
        SDL_Color color;
        SDL_Color color_text;
        bool shown;
        std::vector<std::string> entries;
        int pos;
        int font_size;
        std::string composition;
        int scrollOffset;
    public:
        console(void);
        int initialize();
        void draw(void);
        bool is_open(void);
        void update(void);
        void scroll(float);
        void close(void);
        void open(void);
        void write(std::string);
};

extern console gConsole;

#endif
