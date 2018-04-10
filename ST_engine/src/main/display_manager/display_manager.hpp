#ifndef DM_DEF
#define DM_DEF

#include <string>
#include <defs.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>

class display_manager{
    private:
        SDL_Surface* icon{};
        SDL_Window* window{};
        SDL_DisplayMode DM{};
        task_manager* gTask_manager{};
        message_bus* gMessage_bus{};
        int height = 0;
        int width = 0;
        subscriber* msg_sub{};

        void set_fullscreen(bool arg);
        void handle_messages();
        void set_brightness(float arg);
        static void update_task(void* mngr);

    public:
        display_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void update();
        void close();

        SDL_Window* get_window();
};


//INLINED METHODS

inline void display_manager::update(){
    gTask_manager->start_task_lockfree(new task(update_task, this, nullptr, -1));
}

#endif
