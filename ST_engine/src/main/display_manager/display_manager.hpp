/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef DM_DEF
#define DM_DEF

#include <string>
#include <defs.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>
#include <task_manager/task_allocator.hpp>

///This object is responsible for managing the window.
/**
 * Creates and manages an SDL_Window.
 *
 * Messages this subsystem listens to: <br>
 *
 * <b>SET_FULLSCREEN</b> - Sets the window to fullscreen or windowed mode.
 *
 * Message must contain: a pointer to a <b>bool</b>. <br>
 *
 * <b>SET_WINDOW_BRIGHTNESS</b> - Sets the window brightness. Does not currently work.
 *
 * Message must contain: a pointer to a <b>float</b>. <br>
 *
 */
class display_manager{
    friend class display_manager_tests;
    private:
        SDL_Surface* icon{};
        SDL_Window* window{};
        SDL_DisplayMode DM{};
        task_manager* gTask_manager{};
        message_bus* gMessage_bus{};
        int16_t height = 0;
        int16_t width = 0;
        subscriber msg_sub{};

        void set_fullscreen(bool arg);
        void handle_messages();
        void set_brightness(float arg);
        static void update_task(void* mngr);

    public:
        display_manager(message_bus* msg_bus, task_manager* tsk_mngr);
        ~display_manager();
        void update();
        SDL_Window* get_window();
};


//INLINED METHODS

/**
 * Starts an the update_task() method using the task manager.
 */
inline void display_manager::update(){
    gTask_manager->start_task_lockfree(make_task(update_task, this, nullptr, -1));
}

#endif
