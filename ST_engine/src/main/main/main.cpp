#include <main/main.hpp>

int main(int argc, char** argv){

    //we get rid of the two warnings for unused parameters.
    //int main() will work fine on linux, but not on Windows
    //where SDL does some weird stuff with the main function
    //so we need to specify main like this to avoid conflict with SDL
    (void)argc;
    (void)argv;

    //declare all subsystems
    //order is irrelevant - all constructors are empty
    timer gTimer;
    #ifdef __DEBUG
    fps gFps;
    console gConsole;
    #endif
    task_manager gTask_manager;
    drawing_manager gDrawing_manager;
    message_bus gMessage_bus;
    game_manager gGame_manager;
    input_manager gInput_manager;
    audio_manager gAudio_manager;
    assets_manager gAssets_manager;
    display_manager gDisplay_manager;
    physics_manager gPhysics_manager;

    //Order of initialization however is crucial
    gMessage_bus.initialize();
	gInput_manager.initialize(&gMessage_bus, &gTask_manager);
	gDisplay_manager.initialize(&gMessage_bus, &gTask_manager);
    gDrawing_manager.initialize(gDisplay_manager.get_window(), &gMessage_bus, &gTask_manager);
	gAssets_manager.initialize(&gMessage_bus, &gTask_manager);
    gPhysics_manager.initialize(&gMessage_bus, &gTask_manager);
    gGame_manager.initialize(&gMessage_bus, &gTask_manager); // will load "levels/main"
    gAudio_manager.initialize(&gMessage_bus, &gTask_manager);
    #ifdef __DEBUG
    gConsole.initialize(&gMessage_bus);
    gFps.initialize();
    #endif
    gTask_manager.initialize(&gMessage_bus);
    gTimer.initialize();

    #ifdef __DEBUG
    gConsole.set_log_level(log_type::INFO | log_type::SUCCESS | log_type::ERROR);
    gMessage_bus.send_msg(make_msg(SET_FULLSCREEN, make_data<bool>(false)));
    #elif defined(__RELEASE)
    gMessage_bus.send_msg(make_msg(SET_FULLSCREEN, make_data<bool>(true)));
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

            while (total_time >= UPDATE_RATE) {
                gPhysics_manager.update(gGame_manager.get_level_data());
                gGame_manager.update();
                total_time -= UPDATE_RATE;
            }
            //All three start their own update tasks which run in the background
            gAudio_manager.update();
            gAssets_manager.update();
            gDisplay_manager.update();
        }

        //Update the Console and fps counter in a debug build
        #ifdef __DEBUG
        gConsole.update();
        gFps.update(current_time, 1000/frame_time);
        gDrawing_manager.update(gGame_manager.get_level(), gFps.get_value(), &gConsole);

        #elif defined(__RELEASE)
        gDrawing_manager.update(gGame_manager.get_level(), 0, nullptr);
        #endif
    }

    //Clean up everything and quit subsystems
    //The order here is also important
    gAssets_manager.close();
    gGame_manager.close();
    gPhysics_manager.close();
    gInput_manager.close();
    gAudio_manager.close();
    gDrawing_manager.close();
    gDisplay_manager.close();
    #ifdef __DEBUG
    gConsole.close();
    #endif
    gTask_manager.close();
    gMessage_bus.close();
    return 0;
}
