/** \mainpage This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <main/main.hpp>

#ifdef TESTING
message_bus gMessage_bus;
#endif

/**
 * Execution starting point.
 * Initializes all subsystems and starts the main loop.
 */
#ifndef TESTING
int main(int argc, char *argv[]) {
#elif defined(TESTING)
int ST_engine_main(int argc, char *argv[]) {
#endif
    //we get rid of the two warnings for unused parameters.
    //int main() will work fine on linux, but not on Windows
    //where SDL does some weird stuff with the main function
    //so we need to specify main like this to avoid conflict with SDL
    (void)argc;
    (void)argv;

    //Order of subsystem initialization is crucial
#ifndef TESTING
    message_bus gMessage_bus;
#endif
    fps gFps;
    console gConsole(gMessage_bus);
    gConsole.set_log_level(ST::log_type::INFO | ST::log_type::SUCCESS | ST::log_type::ERROR);

    task_manager gTask_manager;
    audio_manager gAudio_manager(gTask_manager, gMessage_bus);
    input_manager gInput_manager(gTask_manager, gMessage_bus);
    window_manager gDisplay_manager(gMessage_bus, gTask_manager, "ST");
    drawing_manager gDrawing_manager(gDisplay_manager.get_window(), gMessage_bus);

    assets_manager gAssets_manager(gMessage_bus, gTask_manager);
    physics_manager gPhysics_manager(gMessage_bus);
    game_manager gGame_manager(gMessage_bus);// will load "levels/main"
    timer gTimer;

    gConsole.post_init();

    //time keeping variables
    const double LOGIC_UPDATE_RATE = 13;
    double total_time = 0;
    double current_time = gTimer.time_since_start();
    double frame_time;
    double new_time;

    //Temporary fix for an occasional startup crash
    assets_manager::update_task(&gAssets_manager);
    gDisplay_manager.update();

    //main loop
    while(gGame_manager.game_is_running()){
        new_time = gTimer.time_since_start();
        frame_time = new_time - current_time;
        current_time = new_time;
        total_time += frame_time;

        if(total_time >= LOGIC_UPDATE_RATE){
            do{
                gInput_manager.update();
                gGame_manager.update();
                gPhysics_manager.update(&gGame_manager.get_level()->entities);
                total_time -= LOGIC_UPDATE_RATE;
            }
            while (total_time >= LOGIC_UPDATE_RATE);
            //All three start their own update tasks which run in the background
            gAssets_manager.update();
            gDisplay_manager.update();
            gAudio_manager.update();
        }
        gConsole.update();
        gFps.update(current_time, 1000/frame_time);
        gDrawing_manager.update(*gGame_manager.get_level(), gFps.get_value(), gConsole);
    }
    return 0;
}
