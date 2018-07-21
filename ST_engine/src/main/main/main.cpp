/** \mainpage This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <main/main.hpp>

/**
 * Execution starting point.
 * Initializes all subsystems and starts the main loop.
 */
int main(int argc, char** argv){

    //we get rid of the two warnings for unused parameters.
    //int main() will work fine on linux, but not on Windows
    //where SDL does some weird stuff with the main function
    //so we need to specify main like this to avoid conflict with SDL
    (void)argc;
    (void)argv;

    //Order of subsystem initialization is crucial
    message_bus gMessage_bus;
    task_manager gTask_manager(&gMessage_bus);
    input_manager gInput_manager(&gMessage_bus, &gTask_manager);
    #ifdef __DEBUG
    fps gFps;
    console gConsole(&gMessage_bus);
    #endif
    display_manager gDisplay_manager(&gMessage_bus, &gTask_manager);
    drawing_manager gDrawing_manager(gDisplay_manager.get_window(), &gMessage_bus, &gTask_manager);
    assets_manager gAssets_manager(&gMessage_bus, &gTask_manager);
    physics_manager gPhysics_manager(&gMessage_bus, &gTask_manager);
    game_manager gGame_manager(&gMessage_bus, &gTask_manager);// will load "levels/main"
    audio_manager gAudio_manager(&gMessage_bus, &gTask_manager);
    timer gTimer;

    #ifdef __DEBUG
    gConsole.set_log_level(ST::log_type::INFO | ST::log_type::SUCCESS | ST::log_type::ERROR);
    gMessage_bus.send_msg(make_msg(SET_FULLSCREEN, make_data(false)));
    #elif defined(__RELEASE)
    gMessage_bus.send_msg(make_msg(SET_FULLSCREEN, make_data(true)));
    #endif
    gMessage_bus.send_msg(make_msg(VSYNC_ON, nullptr));

    //time keeping variables
    const double UPDATE_RATE = 16.666667; //GAME LOGIC RUNS AT 60 FPS (or less)
    double total_time = 0;
    double current_time = gTimer.time_since_start();
    double frame_time = 0;
    double new_time = 0;

    //main loop
    while(gGame_manager.game_is_running()){
        new_time = gTimer.time_since_start();
        frame_time = new_time - current_time;
        current_time = new_time;
        total_time += frame_time;

        if(total_time >= UPDATE_RATE){
            //will start an update task
            gInput_manager.update();
            do{
                gPhysics_manager.update(gGame_manager.get_level_data());
                gGame_manager.update();
                total_time -= UPDATE_RATE;
            }
            while (total_time >= UPDATE_RATE);
            //All three start their own update tasks which run in the background
            gAudio_manager.update();
            gAssets_manager.update();
            gDisplay_manager.update();
        }

        //Update the Console and fps counter in a debug build
        #ifdef __DEBUG
        gConsole.update();
        gFps.update(current_time, 1000/frame_time);
        gDrawing_manager.update(*gGame_manager.get_level_data(), gFps.get_value(), gConsole);

        #elif defined(__RELEASE)
        gDrawing_manager.update(gGame_manager.get_level_data(), 0, nullptr);
        #endif
    }
    return 0;
}
