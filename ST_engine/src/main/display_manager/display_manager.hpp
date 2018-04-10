#ifndef DM_DEF
#define DM_DEF

#include <string>
#include <defs.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>
#include <manager.hpp>

class display_manager : public virtual manager{
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
        void handle_messages() override;
        void set_brightness(float arg);
        static void update_task(void* mngr);

    public:
        display_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr) override;
        void update() override;
        void close() override;

        SDL_Window* get_window();
};

#endif
