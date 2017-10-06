#include "input_manager.hpp"
#include "level_manager.hpp"

input_manager::input_manager(){
    //empty constructor
}

int input_manager::initialize(){
    int length;
    controls.keyboard = SDL_GetKeyboardState(&length);
    controls.keyboardFramePrev = (Uint8*)malloc(sizeof(Uint8)*length);
    controls.mouseX = event.motion.x;
    controls.mouseY = event.motion.y;
    return 0;
}

void input_manager::take_input(){
    int length;
    SDL_GetKeyboardState(&length);
    for(int i = 0; i < length; i++)
        controls.keyboardFramePrev[i] = controls.keyboard[i];
    for(int i = 0; i < 3; i++)
        controls.mouseClicksFramePrev[i] = 0;
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        controls.mouseClicksFramePrev[0] = 1;
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        controls.mouseClicksFramePrev[1] = 1;
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        controls.mouseClicksFramePrev[2] = 1;
    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT)
            endGame = 0;
        if(event.type == SDL_MOUSEWHEEL)
            controls.mouse_scroll += event.wheel.y;
    }
    for(int i = 0; i < 3; i++)
        controls.mouseClicks[i] = 0;
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        controls.mouseClicks[0] = 1;
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        controls.mouseClicks[1] = 1;
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        controls.mouseClicks[2] = 1;
    controls.mouseX = event.motion.x;
    controls.mouseY = event.motion.y;
}

int input_manager::get_mouseX(){
    return controls.mouseX;
}


int input_manager::get_mouseY(){
    return controls.mouseY;
}

int input_manager::key_pressed(std::string arg){
    std::string key = gLevel_manager.get_level_data()->actions_Buttons[arg];
    return keypress(key);
}

int input_manager::key_released(std::string arg){
    std::string key = gLevel_manager.get_level_data()->actions_Buttons[arg];
    return keyrelease(key);
}

int input_manager::key_held(std::string arg){
    std::string key = gLevel_manager.get_level_data()->actions_Buttons[arg];
    return keyheld(key);
}

int input_manager::keypress(std::string arg){
    int pressed = 0;
    if(arg.compare("left") == 0){
        if(controls.keyboard[SDL_SCANCODE_LEFT] && !controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            pressed = 1;
    }
    else if(arg.compare("right") == 0){
        if(controls.keyboard[SDL_SCANCODE_RIGHT] && !controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            pressed = 1;
    }
    else if(arg.compare("up") == 0){
        if(controls.keyboard[SDL_SCANCODE_UP] && !controls.keyboardFramePrev[SDL_SCANCODE_UP])
            pressed = 1;
    }
    else if(arg.compare("down") == 0){
        if(controls.keyboard[SDL_SCANCODE_DOWN] && !controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            pressed = 1;
    }
    else if(arg.compare("a") == 0){
        if(controls.keyboard[SDL_SCANCODE_A] && !controls.keyboardFramePrev[SDL_SCANCODE_A])
            pressed = 1;
    }
    else if(arg.compare("b") == 0){
        if(controls.keyboard[SDL_SCANCODE_B] && !controls.keyboardFramePrev[SDL_SCANCODE_B])
            pressed = 1;
    }
    else if(arg.compare("c") == 0){
        if(controls.keyboard[SDL_SCANCODE_C] && !controls.keyboardFramePrev[SDL_SCANCODE_C])
            pressed = 1;
    }
    else if(arg.compare("d") == 0){
        if(controls.keyboard[SDL_SCANCODE_D] && !controls.keyboardFramePrev[SDL_SCANCODE_D])
            pressed = 1;
    }
    else if(arg.compare("e") == 0){
        if(controls.keyboard[SDL_SCANCODE_E] && !controls.keyboardFramePrev[SDL_SCANCODE_E])
            pressed = 1;
    }
    else if(arg.compare("f") == 0){
        if(controls.keyboard[SDL_SCANCODE_F] && !controls.keyboardFramePrev[SDL_SCANCODE_F])
            pressed = 1;
    }
    else if(arg.compare("g") == 0){
        if(controls.keyboard[SDL_SCANCODE_G] && !controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = 1;
    }
    else if(arg.compare("h") == 0){
        if(controls.keyboard[SDL_SCANCODE_H] && !controls.keyboardFramePrev[SDL_SCANCODE_H])
            pressed = 1;
    }
    else if(arg.compare("i") == 0){
        if(controls.keyboard[SDL_SCANCODE_I] && !controls.keyboardFramePrev[SDL_SCANCODE_I])
            pressed = 1;
    }
    else if(arg.compare("j") == 0){
        if(controls.keyboard[SDL_SCANCODE_J] && !controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = 1;
    }
    else if(arg.compare("k") == 0){
        if(controls.keyboard[SDL_SCANCODE_K] && !controls.keyboardFramePrev[SDL_SCANCODE_K])
            pressed = 1;
    }
    else if(arg.compare("l") == 0){
        if(controls.keyboard[SDL_SCANCODE_L] && !controls.keyboardFramePrev[SDL_SCANCODE_L])
            pressed = 1;
    }
    else if(arg.compare("m") == 0){
        if(controls.keyboard[SDL_SCANCODE_M] && !controls.keyboardFramePrev[SDL_SCANCODE_M])
            pressed = 1;
    }
    else if(arg.compare("n") == 0){
        if(controls.keyboard[SDL_SCANCODE_N] && !controls.keyboardFramePrev[SDL_SCANCODE_N])
            pressed = 1;
    }
    else if(arg.compare("o") == 0){
        if(controls.keyboard[SDL_SCANCODE_O] && !controls.keyboardFramePrev[SDL_SCANCODE_O])
            pressed = 1;
    }
    else if(arg.compare("p") == 0){
        if(controls.keyboard[SDL_SCANCODE_P] && !controls.keyboardFramePrev[SDL_SCANCODE_P])
            pressed = 1;
    }
    else if(arg.compare("q") == 0){
        if(controls.keyboard[SDL_SCANCODE_Q] && !controls.keyboardFramePrev[SDL_SCANCODE_Q])
            pressed = 1;
    }
    else if(arg.compare("r") == 0){
        if(controls.keyboard[SDL_SCANCODE_R] && !controls.keyboardFramePrev[SDL_SCANCODE_R])
            pressed = 1;
    }
    else if(arg.compare("s") == 0){
        if(controls.keyboard[SDL_SCANCODE_S] && !controls.keyboardFramePrev[SDL_SCANCODE_S])
            pressed = 1;
    }
    else if(arg.compare("t") == 0){
        if(controls.keyboard[SDL_SCANCODE_T] && !controls.keyboardFramePrev[SDL_SCANCODE_T])
            pressed = 1;
    }
    else if(arg.compare("u") == 0){
        if(controls.keyboard[SDL_SCANCODE_U] && !controls.keyboardFramePrev[SDL_SCANCODE_U])
            pressed = 1;
    }
    else if(arg.compare("v") == 0){
        if(controls.keyboard[SDL_SCANCODE_V] && !controls.keyboardFramePrev[SDL_SCANCODE_V])
            pressed = 1;
    }
    else if(arg.compare("w") == 0){
        if(controls.keyboard[SDL_SCANCODE_W] && !controls.keyboardFramePrev[SDL_SCANCODE_W])
            pressed = 1;
    }
    else if(arg.compare("x") == 0){
        if(controls.keyboard[SDL_SCANCODE_X] && !controls.keyboardFramePrev[SDL_SCANCODE_X])
            pressed = 1;
    }
    else if(arg.compare("y") == 0){
        if(controls.keyboard[SDL_SCANCODE_Y] && !controls.keyboardFramePrev[SDL_SCANCODE_Y])
            pressed = 1;
    }
    else if(arg.compare("z") == 0){
        if(controls.keyboard[SDL_SCANCODE_Z] && !controls.keyboardFramePrev[SDL_SCANCODE_Z])
            pressed = 1;
    }
    else if(arg.compare("1") == 0){
        if(controls.keyboard[SDL_SCANCODE_1] && !controls.keyboardFramePrev[SDL_SCANCODE_1])
            pressed = 1;
    }
    else if(arg.compare("2") == 0){
        if(controls.keyboard[SDL_SCANCODE_2] && !controls.keyboardFramePrev[SDL_SCANCODE_2])
            pressed = 1;
    }
    else if(arg.compare("3") == 0){
        if(controls.keyboard[SDL_SCANCODE_3] && !controls.keyboardFramePrev[SDL_SCANCODE_3])
            pressed = 1;
    }
    else if(arg.compare("4") == 0){
        if(controls.keyboard[SDL_SCANCODE_4] && !controls.keyboardFramePrev[SDL_SCANCODE_4])
            pressed = 1;
    }
    else if(arg.compare("5") == 0){
        if(controls.keyboard[SDL_SCANCODE_5] && !controls.keyboardFramePrev[SDL_SCANCODE_5])
            pressed = 1;
    }
    else if(arg.compare("6") == 0){
        if(controls.keyboard[SDL_SCANCODE_6] && !controls.keyboardFramePrev[SDL_SCANCODE_6])
            pressed = 1;
    }
    else if(arg.compare("7") == 0){
        if(controls.keyboard[SDL_SCANCODE_7] && !controls.keyboardFramePrev[SDL_SCANCODE_7])
            pressed = 1;
    }
    else if(arg.compare("8") == 0){
        if(controls.keyboard[SDL_SCANCODE_8] && !controls.keyboardFramePrev[SDL_SCANCODE_8])
            pressed = 1;
    }
    else if(arg.compare("9") == 0){
        if(controls.keyboard[SDL_SCANCODE_9] && !controls.keyboardFramePrev[SDL_SCANCODE_9])
            pressed = 1;
    }
    else if(arg.compare("0") == 0){
        if(controls.keyboard[SDL_SCANCODE_0] && !controls.keyboardFramePrev[SDL_SCANCODE_0])
            pressed = 1;
    }
    else if(arg.compare("escape") == 0){
        if(controls.keyboard[SDL_SCANCODE_ESCAPE] && !controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            pressed = 1;
    }
    else if(arg.compare("enter") == 0){
        if(controls.keyboard[SDL_SCANCODE_RETURN] && !controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            pressed = 1;
    }
    else if(arg.compare("spacebar") == 0){
        if(controls.keyboard[SDL_SCANCODE_SPACE] && !controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            pressed = 1;
    }
    else if(arg.compare("tilde") == 0){
        if(controls.keyboard[SDL_SCANCODE_GRAVE] && !controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            pressed = 1;
    }
    else if(arg.compare("lshift") == 0){
        if(controls.keyboard[SDL_SCANCODE_LSHIFT] && !controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            pressed = 1;
    }
    else if(arg.compare("backspace") == 0){
        if(controls.keyboard[SDL_SCANCODE_BACKSPACE] && !controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            pressed = 1;
    }
    else if(arg.compare("backlash") == 0){
        if(controls.keyboard[SDL_SCANCODE_BACKSLASH] && !controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            pressed = 1;
    }
    else if(arg.compare("capslock") == 0){
        if(controls.keyboard[SDL_SCANCODE_CAPSLOCK] && !controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            pressed = 1;
    }
    else if(arg.compare("comma") == 0){
        if(controls.keyboard[SDL_SCANCODE_COMMA] && !controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            pressed = 1;
    }
    else if(arg.compare("equals") == 0){
        if(controls.keyboard[SDL_SCANCODE_EQUALS] && !controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            pressed = 1;
    }
    else if(arg.compare("leftalt") == 0){
        if(controls.keyboard[SDL_SCANCODE_LALT] && !controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            pressed = 1;
    }
    else if(arg.compare("leftcontrol") == 0){
        if(controls.keyboard[SDL_SCANCODE_LCTRL] && !controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            pressed = 1;
    }
    else if(arg.compare("leftbracket") == 0){
        if(controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && !controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            pressed = 1;
    }
    else if(arg.compare("rightbracket") == 0){
        if(controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && !controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            pressed = 1;
    }
    else if(arg.compare("minus") == 0){
        if(controls.keyboard[SDL_SCANCODE_MINUS] && !controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            pressed = 1;
    }
    else if(arg.compare("rightalt") == 0){
        if(controls.keyboard[SDL_SCANCODE_RALT] && !controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            pressed = 1;
    }
    else if(arg.compare("rightcontrol") == 0){
        if(controls.keyboard[SDL_SCANCODE_RCTRL] && !controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            pressed = 1;
    }
    else if(arg.compare("semicolon") == 0){
        if(controls.keyboard[SDL_SCANCODE_SEMICOLON] && !controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            pressed = 1;
    }
    else if(arg.compare("slash") == 0){
        if(controls.keyboard[SDL_SCANCODE_SLASH] && !controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            pressed = 1;
    }
    else if(arg.compare("tab") == 0){
        if(controls.keyboard[SDL_SCANCODE_TAB] && !controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            pressed = 1;
    }
    else if(arg.compare("mouseLeft") == 0){
        if(!controls.mouseClicksFramePrev[0] && controls.mouseClicks[0])
            pressed = 1;
    }
    else if(arg.compare("mouseMiddle") == 0){
        if(!controls.mouseClicksFramePrev[1] && controls.mouseClicks[1])
            pressed = 1;
    }
    else if(arg.compare("mouseRight") == 0){
        if(!controls.mouseClicksFramePrev[2] && controls.mouseClicks[2])
            pressed = 1;
    }
    return pressed;
}

int input_manager::keyheld(std::string arg){
    int pressed = 0;
    if(arg.compare("left") == 0){
        if(controls.keyboard[SDL_SCANCODE_LEFT] && controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            pressed = 1;
    }
    else if(arg.compare("right") == 0){
        if(controls.keyboard[SDL_SCANCODE_RIGHT] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            pressed = 1;
    }
    else if(arg.compare("up") == 0){
        if(controls.keyboard[SDL_SCANCODE_UP] && controls.keyboardFramePrev[SDL_SCANCODE_UP])
            pressed = 1;
    }
    else if(arg.compare("down") == 0){
        if(controls.keyboard[SDL_SCANCODE_DOWN] && controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            pressed = 1;
    }
    else if(arg.compare("a") == 0){
        if(controls.keyboard[SDL_SCANCODE_A] && controls.keyboardFramePrev[SDL_SCANCODE_A])
            pressed = 1;
    }
    else if(arg.compare("b") == 0){
        if(controls.keyboard[SDL_SCANCODE_B] && controls.keyboardFramePrev[SDL_SCANCODE_B])
            pressed = 1;
    }
    else if(arg.compare("c") == 0){
        if(controls.keyboard[SDL_SCANCODE_C] && controls.keyboardFramePrev[SDL_SCANCODE_C])
            pressed = 1;
    }
    else if(arg.compare("d") == 0){
        if(controls.keyboard[SDL_SCANCODE_D] && controls.keyboardFramePrev[SDL_SCANCODE_D])
            pressed = 1;
    }
    else if(arg.compare("e") == 0){
        if(controls.keyboard[SDL_SCANCODE_E] && controls.keyboardFramePrev[SDL_SCANCODE_E])
            pressed = 1;
    }
    else if(arg.compare("f") == 0){
        if(controls.keyboard[SDL_SCANCODE_F] && controls.keyboardFramePrev[SDL_SCANCODE_F])
            pressed = 1;
    }
    else if(arg.compare("g") == 0){
        if(controls.keyboard[SDL_SCANCODE_G] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = 1;
    }
    else if(arg.compare("h") == 0){
        if(controls.keyboard[SDL_SCANCODE_H] && controls.keyboardFramePrev[SDL_SCANCODE_H])
            pressed = 1;
    }
    else if(arg.compare("i") == 0){
        if(controls.keyboard[SDL_SCANCODE_I] && controls.keyboardFramePrev[SDL_SCANCODE_I])
            pressed = 1;
    }
    else if(arg.compare("j") == 0){
        if(controls.keyboard[SDL_SCANCODE_J] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            pressed = 1;
    }
    else if(arg.compare("k") == 0){
        if(controls.keyboard[SDL_SCANCODE_K] && controls.keyboardFramePrev[SDL_SCANCODE_K])
            pressed = 1;
    }
    else if(arg.compare("l") == 0){
        if(controls.keyboard[SDL_SCANCODE_L] && controls.keyboardFramePrev[SDL_SCANCODE_L])
            pressed = 1;
    }
    else if(arg.compare("m") == 0){
        if(controls.keyboard[SDL_SCANCODE_M] && controls.keyboardFramePrev[SDL_SCANCODE_M])
            pressed = 1;
    }
    else if(arg.compare("n") == 0){
        if(controls.keyboard[SDL_SCANCODE_N] && controls.keyboardFramePrev[SDL_SCANCODE_N])
            pressed = 1;
    }
    else if(arg.compare("o") == 0){
        if(controls.keyboard[SDL_SCANCODE_O] && controls.keyboardFramePrev[SDL_SCANCODE_O])
            pressed = 1;
    }
    else if(arg.compare("p") == 0){
        if(controls.keyboard[SDL_SCANCODE_P] && controls.keyboardFramePrev[SDL_SCANCODE_P])
            pressed = 1;
    }
    else if(arg.compare("q") == 0){
        if(controls.keyboard[SDL_SCANCODE_Q] && controls.keyboardFramePrev[SDL_SCANCODE_Q])
            pressed = 1;
    }
    else if(arg.compare("r") == 0){
        if(controls.keyboard[SDL_SCANCODE_R] && controls.keyboardFramePrev[SDL_SCANCODE_R])
            pressed = 1;
    }
    else if(arg.compare("s") == 0){
        if(controls.keyboard[SDL_SCANCODE_S] && controls.keyboardFramePrev[SDL_SCANCODE_S])
            pressed = 1;
    }
    else if(arg.compare("t") == 0){
        if(controls.keyboard[SDL_SCANCODE_T] && controls.keyboardFramePrev[SDL_SCANCODE_T])
            pressed = 1;
    }
    else if(arg.compare("u") == 0){
        if(controls.keyboard[SDL_SCANCODE_U] && controls.keyboardFramePrev[SDL_SCANCODE_U])
            pressed = 1;
    }
    else if(arg.compare("v") == 0){
        if(controls.keyboard[SDL_SCANCODE_V] && controls.keyboardFramePrev[SDL_SCANCODE_V])
            pressed = 1;
    }
    else if(arg.compare("w") == 0){
        if(controls.keyboard[SDL_SCANCODE_W] && controls.keyboardFramePrev[SDL_SCANCODE_W])
            pressed = 1;
    }
    else if(arg.compare("x") == 0){
        if(controls.keyboard[SDL_SCANCODE_X] && controls.keyboardFramePrev[SDL_SCANCODE_X])
            pressed = 1;
    }
    else if(arg.compare("y") == 0){
        if(controls.keyboard[SDL_SCANCODE_Y] && controls.keyboardFramePrev[SDL_SCANCODE_Y])
            pressed = 1;
    }
    else if(arg.compare("z") == 0){
        if(controls.keyboard[SDL_SCANCODE_Z] && controls.keyboardFramePrev[SDL_SCANCODE_Z])
            pressed = 1;
    }
    else if(arg.compare("1") == 0){
        if(controls.keyboard[SDL_SCANCODE_1] && controls.keyboardFramePrev[SDL_SCANCODE_1])
            pressed = 1;
    }
    else if(arg.compare("2") == 0){
        if(controls.keyboard[SDL_SCANCODE_2] && controls.keyboardFramePrev[SDL_SCANCODE_2])
            pressed = 1;
    }
    else if(arg.compare("3") == 0){
        if(controls.keyboard[SDL_SCANCODE_3] && controls.keyboardFramePrev[SDL_SCANCODE_3])
            pressed = 1;
    }
    else if(arg.compare("4") == 0){
        if(controls.keyboard[SDL_SCANCODE_4] && controls.keyboardFramePrev[SDL_SCANCODE_4])
            pressed = 1;
    }
    else if(arg.compare("5") == 0){
        if(controls.keyboard[SDL_SCANCODE_5] && controls.keyboardFramePrev[SDL_SCANCODE_5])
            pressed = 1;
    }
    else if(arg.compare("6") == 0){
        if(controls.keyboard[SDL_SCANCODE_6] && controls.keyboardFramePrev[SDL_SCANCODE_6])
            pressed = 1;
    }
    else if(arg.compare("7") == 0){
        if(controls.keyboard[SDL_SCANCODE_7] && controls.keyboardFramePrev[SDL_SCANCODE_7])
            pressed = 1;
    }
    else if(arg.compare("8") == 0){
        if(controls.keyboard[SDL_SCANCODE_8] && controls.keyboardFramePrev[SDL_SCANCODE_8])
            pressed = 1;
    }
    else if(arg.compare("9") == 0){
        if(controls.keyboard[SDL_SCANCODE_9] && controls.keyboardFramePrev[SDL_SCANCODE_9])
            pressed = 1;
    }
    else if(arg.compare("0") == 0){
        if(controls.keyboard[SDL_SCANCODE_0] && controls.keyboardFramePrev[SDL_SCANCODE_0])
            pressed = 1;
    }
    else if(arg.compare("escape") == 0){
        if(controls.keyboard[SDL_SCANCODE_ESCAPE] && controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            pressed = 1;
    }
    else if(arg.compare("enter") == 0){
        if(controls.keyboard[SDL_SCANCODE_RETURN] && controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            pressed = 1;
    }
    else if(arg.compare("spacebar") == 0){
        if(controls.keyboard[SDL_SCANCODE_SPACE] && controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            pressed = 1;
    }
    else if(arg.compare("tilde") == 0){
        if(controls.keyboard[SDL_SCANCODE_GRAVE] && controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            pressed = 1;
    }
    else if(arg.compare("lshift") == 0){
        if(controls.keyboard[SDL_SCANCODE_LSHIFT] && controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            pressed = 1;
    }
    else if(arg.compare("backspace") == 0){
        if(controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            pressed = 1;
    }
    else if(arg.compare("backlash") == 0){
        if(controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            pressed = 1;
    }
    else if(arg.compare("capslock") == 0){
        if(controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            pressed = 1;
    }
    else if(arg.compare("comma") == 0){
        if(controls.keyboard[SDL_SCANCODE_COMMA] && controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            pressed = 1;
    }
    else if(arg.compare("equals") == 0){
        if(controls.keyboard[SDL_SCANCODE_EQUALS] && controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            pressed = 1;
    }
    else if(arg.compare("leftalt") == 0){
        if(controls.keyboard[SDL_SCANCODE_LALT] && controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            pressed = 1;
    }
    else if(arg.compare("leftcontrol") == 0){
        if(controls.keyboard[SDL_SCANCODE_LCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            pressed = 1;
    }
    else if(arg.compare("leftbracket") == 0){
        if(controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            pressed = 1;
    }
    else if(arg.compare("rightbracket") == 0){
        if(controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            pressed = 1;
    }
    else if(arg.compare("minus") == 0){
        if(controls.keyboard[SDL_SCANCODE_MINUS] && controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            pressed = 1;
    }
    else if(arg.compare("rightalt") == 0){
        if(controls.keyboard[SDL_SCANCODE_RALT] && controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            pressed = 1;
    }
    else if(arg.compare("rightcontrol") == 0){
        if(controls.keyboard[SDL_SCANCODE_RCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            pressed = 1;
    }
    else if(arg.compare("semicolon") == 0){
        if(controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            pressed = 1;
    }
    else if(arg.compare("slash") == 0){
        if(controls.keyboard[SDL_SCANCODE_SLASH] && controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            pressed = 1;
    }
    else if(arg.compare("tab") == 0){
        if(controls.keyboard[SDL_SCANCODE_TAB] && controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            pressed = 1;
    }
    else if(arg.compare("mouseLeft") == 0){
        if(controls.mouseClicksFramePrev[0] && controls.mouseClicks[0])
            pressed = 1;
    }
    else if(arg.compare("mouseMiddle") == 0){
        if(controls.mouseClicksFramePrev[1] && controls.mouseClicks[1])
            pressed = 1;
    }
    else if(arg.compare("mouseRight") == 0){
        if(controls.mouseClicksFramePrev[2] && controls.mouseClicks[2])
            pressed = 1;
    }
    return pressed;
}

int input_manager::keyrelease(std::string arg){
    int released = 0;
    if(arg.compare("left") == 0){
        if(!controls.keyboard[SDL_SCANCODE_LEFT] && controls.keyboardFramePrev[SDL_SCANCODE_LEFT])
            released = 1;
    }
    else if(arg.compare("right") == 0){
        if(!controls.keyboard[SDL_SCANCODE_RIGHT] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHT])
            released = 1;
    }
    else if(arg.compare("up") == 0){
        if(!controls.keyboard[SDL_SCANCODE_UP] && controls.keyboardFramePrev[SDL_SCANCODE_UP])
            released = 1;
    }
    else if(arg.compare("down") == 0){
        if(!controls.keyboard[SDL_SCANCODE_DOWN] && controls.keyboardFramePrev[SDL_SCANCODE_DOWN])
            released = 1;
    }
    else if(arg.compare("a") == 0){
        if(!controls.keyboard[SDL_SCANCODE_A] && controls.keyboardFramePrev[SDL_SCANCODE_A])
            released = 1;
    }
    else if(arg.compare("b") == 0){
        if(!controls.keyboard[SDL_SCANCODE_B] && controls.keyboardFramePrev[SDL_SCANCODE_B])
            released = 1;
    }
    else if(arg.compare("c") == 0){
        if(!controls.keyboard[SDL_SCANCODE_C] && controls.keyboardFramePrev[SDL_SCANCODE_C])
            released = 1;
    }
    else if(arg.compare("d") == 0){
        if(!controls.keyboard[SDL_SCANCODE_D] && controls.keyboardFramePrev[SDL_SCANCODE_D])
            released = 1;
    }
    else if(arg.compare("e") == 0){
        if(!controls.keyboard[SDL_SCANCODE_E] && controls.keyboardFramePrev[SDL_SCANCODE_E])
            released = 1;
    }
    else if(arg.compare("f") == 0){
        if(!controls.keyboard[SDL_SCANCODE_F] && controls.keyboardFramePrev[SDL_SCANCODE_F])
            released = 1;
    }
    else if(arg.compare("g") == 0){
        if(!controls.keyboard[SDL_SCANCODE_G] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            released = 1;
    }
    else if(arg.compare("h") == 0){
        if(!controls.keyboard[SDL_SCANCODE_H] && controls.keyboardFramePrev[SDL_SCANCODE_H])
            released = 1;
    }
    else if(arg.compare("i") == 0){
        if(!controls.keyboard[SDL_SCANCODE_I] && controls.keyboardFramePrev[SDL_SCANCODE_I])
            released = 1;
    }
    else if(arg.compare("j") == 0){
        if(!controls.keyboard[SDL_SCANCODE_J] && controls.keyboardFramePrev[SDL_SCANCODE_G])
            released = 1;
    }
    else if(arg.compare("k") == 0){
        if(!controls.keyboard[SDL_SCANCODE_K] && controls.keyboardFramePrev[SDL_SCANCODE_K])
            released = 1;
    }
    else if(arg.compare("l") == 0){
        if(!controls.keyboard[SDL_SCANCODE_L] && controls.keyboardFramePrev[SDL_SCANCODE_L])
            released = 1;
    }
    else if(arg.compare("m") == 0){
        if(!controls.keyboard[SDL_SCANCODE_M] && controls.keyboardFramePrev[SDL_SCANCODE_M])
            released = 1;
    }
    else if(arg.compare("n") == 0){
        if(!controls.keyboard[SDL_SCANCODE_N] && controls.keyboardFramePrev[SDL_SCANCODE_N])
            released = 1;
    }
    else if(arg.compare("o") == 0){
        if(!controls.keyboard[SDL_SCANCODE_O] && controls.keyboardFramePrev[SDL_SCANCODE_O])
            released = 1;
    }
    else if(arg.compare("p") == 0){
        if(!controls.keyboard[SDL_SCANCODE_P] && controls.keyboardFramePrev[SDL_SCANCODE_P])
            released = 1;
    }
    else if(arg.compare("q") == 0){
        if(!controls.keyboard[SDL_SCANCODE_Q] && controls.keyboardFramePrev[SDL_SCANCODE_Q])
            released = 1;
    }
    else if(arg.compare("r") == 0){
        if(!controls.keyboard[SDL_SCANCODE_R] && controls.keyboardFramePrev[SDL_SCANCODE_R])
            released = 1;
    }
    else if(arg.compare("s") == 0){
        if(!controls.keyboard[SDL_SCANCODE_S] && controls.keyboardFramePrev[SDL_SCANCODE_S])
            released = 1;
    }
    else if(arg.compare("t") == 0){
        if(!controls.keyboard[SDL_SCANCODE_T] && controls.keyboardFramePrev[SDL_SCANCODE_T])
            released = 1;
    }
    else if(arg.compare("u") == 0){
        if(!controls.keyboard[SDL_SCANCODE_U] && controls.keyboardFramePrev[SDL_SCANCODE_U])
            released = 1;
    }
    else if(arg.compare("v") == 0){
        if(!controls.keyboard[SDL_SCANCODE_V] && controls.keyboardFramePrev[SDL_SCANCODE_V])
            released = 1;
    }
    else if(arg.compare("w") == 0){
        if(!controls.keyboard[SDL_SCANCODE_W] && controls.keyboardFramePrev[SDL_SCANCODE_W])
            released = 1;
    }
    else if(arg.compare("x") == 0){
        if(!controls.keyboard[SDL_SCANCODE_X] && controls.keyboardFramePrev[SDL_SCANCODE_X])
            released = 1;
    }
    else if(arg.compare("y") == 0){
        if(!controls.keyboard[SDL_SCANCODE_Y] && controls.keyboardFramePrev[SDL_SCANCODE_Y])
            released = 1;
    }
    else if(arg.compare("z") == 0){
        if(!controls.keyboard[SDL_SCANCODE_Z] && controls.keyboardFramePrev[SDL_SCANCODE_Z])
            released = 1;
    }
    else if(arg.compare("1") == 0){
        if(!controls.keyboard[SDL_SCANCODE_1] && controls.keyboardFramePrev[SDL_SCANCODE_1])
            released = 1;
    }
    else if(arg.compare("2") == 0){
        if(!controls.keyboard[SDL_SCANCODE_2] && controls.keyboardFramePrev[SDL_SCANCODE_2])
            released = 1;
    }
    else if(arg.compare("3") == 0){
        if(!controls.keyboard[SDL_SCANCODE_3] && controls.keyboardFramePrev[SDL_SCANCODE_3])
            released = 1;
    }
    else if(arg.compare("4") == 0){
        if(!controls.keyboard[SDL_SCANCODE_4] && controls.keyboardFramePrev[SDL_SCANCODE_4])
            released = 1;
    }
    else if(arg.compare("5") == 0){
        if(!controls.keyboard[SDL_SCANCODE_5] && controls.keyboardFramePrev[SDL_SCANCODE_5])
            released = 1;
    }
    else if(arg.compare("6") == 0){
        if(!controls.keyboard[SDL_SCANCODE_6] && controls.keyboardFramePrev[SDL_SCANCODE_6])
            released = 1;
    }
    else if(arg.compare("7") == 0){
        if(!controls.keyboard[SDL_SCANCODE_7] && controls.keyboardFramePrev[SDL_SCANCODE_7])
            released = 1;
    }
    else if(arg.compare("8") == 0){
        if(!controls.keyboard[SDL_SCANCODE_8] && controls.keyboardFramePrev[SDL_SCANCODE_8])
            released = 1;
    }
    else if(arg.compare("9") == 0){
        if(!controls.keyboard[SDL_SCANCODE_9] && controls.keyboardFramePrev[SDL_SCANCODE_9])
            released = 1;
    }
    else if(arg.compare("0") == 0){
        if(!controls.keyboard[SDL_SCANCODE_0] && controls.keyboardFramePrev[SDL_SCANCODE_0])
            released = 1;
    }
    else if(arg.compare("escape") == 0){
        if(!controls.keyboard[SDL_SCANCODE_ESCAPE] && controls.keyboardFramePrev[SDL_SCANCODE_ESCAPE])
            released = 1;
    }
    else if(arg.compare("enter") == 0){
        if(!controls.keyboard[SDL_SCANCODE_RETURN] && controls.keyboardFramePrev[SDL_SCANCODE_RETURN])
            released = 1;
    }
    else if(arg.compare("spacebar") == 0){
        if(!controls.keyboard[SDL_SCANCODE_SPACE] && controls.keyboardFramePrev[SDL_SCANCODE_SPACE])
            released = 1;
    }
    else if(arg.compare("tilde") == 0){
        if(!controls.keyboard[SDL_SCANCODE_GRAVE] && controls.keyboardFramePrev[SDL_SCANCODE_GRAVE])
            released = 1;
    }
    else if(arg.compare("lshift") == 0){
        if(!controls.keyboard[SDL_SCANCODE_LSHIFT] && controls.keyboardFramePrev[SDL_SCANCODE_LSHIFT])
            released = 1;
    }
    else if(arg.compare("backspace") == 0){
        if(!controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSPACE])
            released = 1;
    }
    else if(arg.compare("backlash") == 0){
        if(!controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls.keyboardFramePrev[SDL_SCANCODE_BACKSLASH])
            pressed = 1;
    }
    else if(arg.compare("capslock") == 0){
        if(!controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls.keyboardFramePrev[SDL_SCANCODE_CAPSLOCK])
            pressed = 1;
    }
    else if(arg.compare("comma") == 0){
        if(!controls.keyboard[SDL_SCANCODE_COMMA] && controls.keyboardFramePrev[SDL_SCANCODE_COMMA])
            pressed = 1;
    }
    else if(arg.compare("equals") == 0){
        if(!controls.keyboard[SDL_SCANCODE_EQUALS] && controls.keyboardFramePrev[SDL_SCANCODE_EQUALS])
            pressed = 1;
    }
    else if(arg.compare("leftalt") == 0){
        if(!controls.keyboard[SDL_SCANCODE_LALT] && controls.keyboardFramePrev[SDL_SCANCODE_LALT])
            pressed = 1;
    }
    else if(arg.compare("leftcontrol") == 0){
        if(!controls.keyboard[SDL_SCANCODE_LCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_LCTRL])
            pressed = 1;
    }
    else if(arg.compare("leftbracket") == 0){
        if(!controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_LEFTBRACKET])
            pressed = 1;
    }
    else if(arg.compare("rightbracket") == 0){
        if(!controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls.keyboardFramePrev[SDL_SCANCODE_RIGHTBRACKET])
            pressed = 1;
    }
    else if(arg.compare("minus") == 0){
        if(!controls.keyboard[SDL_SCANCODE_MINUS] && controls.keyboardFramePrev[SDL_SCANCODE_MINUS])
            pressed = 1;
    }
    else if(arg.compare("rightalt") == 0){
        if(!controls.keyboard[SDL_SCANCODE_RALT] && controls.keyboardFramePrev[SDL_SCANCODE_RALT])
            pressed = 1;
    }
    else if(arg.compare("rightcontrol") == 0){
        if(!controls.keyboard[SDL_SCANCODE_RCTRL] && controls.keyboardFramePrev[SDL_SCANCODE_RCTRL])
            pressed = 1;
    }
    else if(arg.compare("semicolon") == 0){
        if(!controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls.keyboardFramePrev[SDL_SCANCODE_SEMICOLON])
            pressed = 1;
    }
    else if(arg.compare("slash") == 0){
        if(!controls.keyboard[SDL_SCANCODE_SLASH] && controls.keyboardFramePrev[SDL_SCANCODE_SLASH])
            pressed = 1;
    }
    else if(arg.compare("tab") == 0){
        if(!controls.keyboard[SDL_SCANCODE_TAB] && controls.keyboardFramePrev[SDL_SCANCODE_TAB])
            pressed = 1;
    }
    else if(arg.compare("mouseLeft") == 0){
        if(controls.mouseClicksFramePrev[0] && !controls.mouseClicks[0])
            released = 1;
    }
    else if(arg.compare("mouseMiddle") == 0){
        if(controls.mouseClicksFramePrev[1] && !controls.mouseClicks[1])
            released = 1;
    }
    else if(arg.compare("mouseRight") == 0){
        if(controls.mouseClicksFramePrev[2] && !controls.mouseClicks[2])
            released = 1;
    }
    return released;
}

void input_manager::close(){
    free(controls.keyboardFramePrev);
}
