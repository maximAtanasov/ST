#include "main.hpp"
#include "timer.hpp"

int endGame = 1;
Uint32 ticks; //CPU ticks since start

display_manager gDisplay_manager;
audio_manager gAudio_manager;
level_manager gLevel_manager;
input_manager gInput_manager;
assets_manager gAssets_manager;
lua_manager gLua_manager;
renderer gRenderer;
console gConsole;
timer gTimer;

int main(int argc, char *argv[]){

    //initialize all subsystems
    gDisplay_manager.initialize();
    gAudio_manager.initialize();
    gLevel_manager.initialize();
    gInput_manager.initialize();
    gLua_manager.initialize();
    gRenderer.initialize();
    gAssets_manager.initialize();
    gConsole.initialize();
    gTimer.initialize();

    gDisplay_manager.set_fullscreen();
    gRenderer.vsync_on();

    //seed random generator
    srand(time(NULL));

    //Load first level
    gLevel_manager.load_level("main");
    gLevel_manager.start_level("main");

    const double UPDATE_STEP = 16.666667; //GAME LOGIC RUNS AT 60 FPS (or less)
    double total_time = 0;
    double current_time = gTimer.time_since_start();
    double frame_time = 0;
    double new_time;
    SDL_Color color_font = {0, 0, 255, 255};

    //main loop
    while(endGame){ //the order of things in here is crucial
        ticks = SDL_GetTicks();
        new_time = gTimer.time_since_start();
        frame_time = new_time - current_time;
        current_time = new_time;
        total_time += frame_time;
        gRenderer.clear_screen();
        while(total_time >= UPDATE_STEP){
            gInput_manager.take_input();
            gLevel_manager.run_level_loop();
            total_time -= UPDATE_STEP;
        }
        draw(0);
        gConsole.update();
        //fps counter
        gRenderer.draw_text("fonts/OpenSans-Regular.ttf", "fps: " + std::to_string((int)(1000.0/(double)frame_time)), 0, 0, color_font, 40, D_RENDERTEXT_FAST);
        //gRenderer.draw_text("fonts/OpenSans-Regular.ttf", "ms per frame: " + std::to_string(frame_time), 0, 40, color_font, 40, D_RENDERTEXT_FAST);

        gRenderer.present();
    }

    //Clean up everything and quit subsystems
    gLevel_manager.close();
    gRenderer.close();
    gAudio_manager.close();
    gDisplay_manager.close();
    gInput_manager.close();
    gLua_manager.close();
}

inline void draw(double extrapolation){
    draw_entities();
    if(collisionsShown){
        drawCollisions();
        drawCoordinates();
    }
}

//this is a proper mess lol - gotta fix it
void drawEntities(){
    gRenderer.draw_background(gLevel_manager.get_level_data()->background);
    for(std::vector<entity*>::iterator i = gLevel_manager.get_level_data()->entities.begin(); i != gLevel_manager.get_level_data()->entities.end(); i++){
        if((*i)->is_onscreen()){
            if((*i)->getAnimation() == 0)
                (*i)->draw();
            else
                (*i)->drawAnimation();
        }
    }
}

void drawCollisions(){
    for(std::vector<entity*>::iterator i = gLevel_manager.get_level_data()->entities.begin(); i != gLevel_manager.get_level_data()->entities.end(); i++)
        if((*i)->is_onscreen())
            (*i)->drawCollisionBox();
}

void drawCoordinates(void){
    for(std::vector<entity*>::iterator i = gLevel_manager.get_level_data()->entities.begin(); i != gLevel_manager.get_level_data()->entities.end(); i++)
    if((*i)->is_onscreen())
        if((*i)->isAffectedByPhysics())
            (*i)->drawCoordinates();
}
