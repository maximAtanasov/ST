/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef DRAWING_DEF
#define DRAWING_DEF

#include <game_manager/level/light.hpp>
#include <message_bus.hpp>
#include <game_manager/level/camera.hpp>
#include <renderer_sdl.hpp>
#include <game_manager/level/level.hpp>
#include <console.hpp>
#include <task_manager.hpp>


#define DEFAULT_FONT_NORMAL "OpenSans-Regular.ttf 40"
#define DEFAULT_FONT_SMALL "OpenSans-Regular.ttf 40"

///This object is responsible for issuing drawing commands and drawing the current level.
class drawing_manager{
    private:
        //external dependency - delivered in the constructor
        message_bus& gMessage_bus;
        task_manager& gTask_manager;
        SDL_Window* window{};

        //a subscriber object - so we can subscribe to and recieve messages
        subscriber msg_sub{};

        //CPU ticks since start - used for animating sprites
        uint32_t ticks = 0;

        //Basically the viewport
        ST::camera camera{};

        //Internal rendering resolution
        uint16_t w_width = 1920;
        uint16_t w_height = 1080;

        //variables for drawing light
        uint8_t lightmap[1920][1080]{};
        uint8_t darkness_level = 0;
        uint8_t lights_quality = 0;

        uint16_t default_font_normal = 0;
        uint16_t default_font_small = 0;

        //debug
        bool collisions_shown = false;
        bool show_fps = true;
        bool lighting_enabled = true;

        //Level and dev console state
        ST::level level = ST::level("", &gMessage_bus); //Initial level state - empty
        double fps = 0;
        console* cnsl{};

        //Drawing functions
        void draw_entities() const;
        void draw_collisions() const;
        void draw_coordinates() const;
        void draw_lights() const;
        void draw_fps() const;
        void draw_console();
        void draw_text_objects() const;

        //Pre-processing
        void process_lights(const std::vector<ST::light>& arg);
        [[nodiscard]] bool is_onscreen(const ST::entity& i) const;
        [[nodiscard]] bool is_onscreen(const ST::text& i) const;

        //Other functions
        void handle_messages();
        void set_darkness(uint8_t arg);
        void copy_level_state(ST::level *level);

public:
        drawing_manager(message_bus &gMessageBus, task_manager &gTaskManager);
        ~drawing_manager();
        task_id initialize(SDL_Window *window);
        task_id update(ST::level* temp, double, console* gConsole);
        static void update_task(void* self);


    static void init_task(void *arg);
};

#endif
