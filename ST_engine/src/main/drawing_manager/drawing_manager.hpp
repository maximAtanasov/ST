#ifndef DRAWING_DEF
#define DRAWING_DEF

#include <defs.hpp>
#include <renderers/renderer_sdl/font_cache.hpp>
#include <assets_manager/assets.hpp>
#include <game_manager/level/level.hpp>
#include <game_manager/level/light.hpp>
#include <console/console.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>
#include <game_manager/level/camera.hpp>
#include <renderers/renderer_sdl/renderer_sdl.hpp>

class drawing_manager : public virtual manager{
    private:
        //external dependencies - injected through main or the message bus
        message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
		ST::assets* asset_ptr{};
		SDL_Window* window{};

        //a subscriber object - so we can subscribe to and recieve messages
        subscriber* msg_sub{};

        //rendering object - the interface
        renderer_sdl* gRenderer{};

        //CPU ticks since start - used for animating sprites
        Uint32 ticks = 0;


        //basically the argument passed to process_lights
        std::vector<light>* lights_arg{};

        //Basically the viewport
        ST::camera Camera{};

        //Internal rendering resolution
        const int w_width = 1920;
        const int w_height = 1080;

        //variables for drawing light
        Uint8 lightmap[1920][1080];
        Uint8 lightmap_rotated[1080][1920];
        Uint8 darkness_level = 0;
        int lights_quality = 0;
		
        //debug
        bool collisions_shown = false;
        bool show_fps = true;
        bool lighting_enabled = true;

        //Drawing functions
        void draw_entities(std::vector<entity>*);
        void draw_collisions(std::vector<entity>*);
        void draw_coordinates(std::vector<entity>*);
        void draw_lights();
        void draw_fps(double fps);
        void draw_console(console* cnsl);
        void draw_text_objects(std::vector<text>*);

        //Pre-processing
        void rotate_lightmap();
        void process_lights(std::vector<light>* arg);
        bool is_onscreen(entity* i);

        //Other functions
        void handle_messages() override;
        void show_collisions();
        void hide_collisions();
        void set_darkness(Uint8 arg);
        static void process_lights_task(void* arg);

    public:
        drawing_manager() = default; //empty constructor
        int initialize(SDL_Window* win, message_bus* msg_bus, task_manager* tsk_mngr);
        void update(level temp, double, console* gConsole);

        //Yes, this manager is bit special in that it overloads the initialize and update methods
        int initialize(message_bus*, task_manager*) override {return 0;};
        void update() override {};
        void close() override;
};

#endif
