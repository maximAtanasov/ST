#ifndef MSG_BUS_DEF
#define MSG_BUS_DEF

#include <cstdlib>
#include <iostream>
#include "../util/atomic_queue/concurrentqueue.h"
#include <unordered_map>
#include <SDL2/SDL.h>
#include <memory>

//all the different messages are defined in this enum
//to define a new one just add its name here - they are all used as integers

enum msg_type {VSYNC_ON, VSYNC_OFF, SET_DARKNESS,
                PLAY_SOUND, PLAY_MUSIC, STOP_MUSIC, TOGGLE_AUDIO,
                LOAD_LEVEL, START_LEVEL, UNLOAD_LEVEL,
                CONSOLE_WRITE, CONSOLE_TOGGLE, SHOW_COLLISIONS,
                LOAD_LIST, UNLOAD_LIST, ASSETS, KEY_PRESSED, KEY_HELD,
                KEY_RELEASED, MOUSE_X, MOUSE_Y, MOUSE_SCROLL, SET_GRAVITY, SET_FRICTION,
                SET_FLOOR, VSYNC_IS_OFF, VSYNC_IS_ON, END_GAME, VOLUME_LEVEL, STOP_ALL_SOUNDS,
				VIRTUAL_SCREEN_COORDINATES, REAL_SCREEN_COORDINATES, PAUSE_PHYSICS, UNPAUSE_PHYSICS,
				RENDERER_SWITCH, START_TEXT_INPUT, STOP_TEXT_INPUT, TEXT_STREAM, CLEAR_TEXT_STREAM, SHOW_MOUSE,SHOW_FPS,
                ENABLE_LIGHTING, REGISTER_KEY, UNREGISTER_KEY, SET_VOLUME, SET_WINDOW_BRIGHTNESS, EXECUTE_SCRIPT,
				SET_FULLSCREEN, FULLSCREEN_STATUS, LOAD_ASSET, UNLOAD_ASSET, LOAD_BINARY
			};

//Never use new() or delete() or the constructor  (it will just break) with this class - only make_msg() and destroy_msg()
class message{
    private:
		int id; //used during allocation and deallocation
        std::shared_ptr<void> data; //yes, this holds anything created with make_data<>() AND calls the correct destructor
                                    //that's how shared_ptr works, if you don't believe me, well google it or something
    public:
        msg_type msg_name;
		int get_id();
        message(msg_type name, const std::shared_ptr<void>& data, int id);
        void* get_data();
        message* make_copy();
};

class message_allocator{
    private:
        SDL_mutex* access_mutex{};
        int pointer = 0;
        message* memory{};
        int memory_size = 300;
        bool allocated[300]{};

    public:
        message_allocator();
        message* allocate_message(msg_type name, std::shared_ptr<void> data);
        void deallocate(int id, message* temp);
        ~message_allocator();
};

class subscriber{
    private:
        moodycamel::ConcurrentQueue<message*> queue;
    public:
        subscriber() = default;
        message* get_next_message();
        void push_message(message* arg);
};

class message_bus{
    private:
        std::unordered_map<int, std::vector<subscriber*>> subscribers; //each message enum maps to a list of subscribers for that message
    public:
        message_bus() = default;
        int initialize();
        void send_msg(message* msg);
        void subscribe(msg_type msg, subscriber* sub);
        void close();
};


//To create a message use "make_msg" along with "make_data<>" - you do not need to manage the memory of the data
//only the lifetime of a message
//Any data you get out of a message (using get_data()) is guaranteed to be available until you call "destroy_msg()" on that message
//afterwards the results are undefined
//shared pointer are used internally to manage memory
message* make_msg(msg_type name, const std::shared_ptr<void>& data);
void destroy_msg(message* msg);

template <class T> std::shared_ptr<void> make_data(T data){
    return std::make_shared<T>(data);
}

#endif
