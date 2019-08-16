/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef WM_DEF
#define WM_DEF

#include <string>
#include <message_bus.hpp>
#include <task_manager.hpp>
#include <SDL_surface.h>
#include <SDL_video.h>

///This object is responsible for managing the window.
class window_manager{
    private:
        SDL_Surface* icon{};
        SDL_Window* window{};
        SDL_DisplayMode DM{};
        task_manager& gTask_manager;
        message_bus& gMessage_bus;
        int16_t height = 0;
        int16_t width = 0;
        subscriber msg_sub{};

        void set_fullscreen(bool arg);
        void handle_messages();
        void set_brightness(float arg);
        static void update_task(void* mngr);

    public:
        window_manager(message_bus &gMessageBus, task_manager &gTask_manager, const std::string &window_name);
        ~window_manager();
        void update();
        SDL_Window* get_window();
};


//INLINED METHODS

/**
 * Starts an the update_task() method using the task manager.
 */
inline void window_manager::update(){
    gTask_manager.start_task_lockfree(new ST::task(update_task, this, nullptr));
}

#endif
