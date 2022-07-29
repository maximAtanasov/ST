/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <drawing_manager/drawing_manager.hpp>
#include <ST_util/string_util.hpp>
#include "main/metrics.hpp"
#include "main/timer.hpp"

static bool singleton_initialized = false;

/**
 *
 * @param window A pointer to an SDL_Window to bind the renderer to.
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_manager.
 */
drawing_manager::drawing_manager(SDL_Window *window, message_bus &gMessageBus) : gMessage_bus(gMessageBus) {

    if(singleton_initialized){
        throw std::runtime_error("The drawing manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    if(TTF_Init() < 0){
        fprintf(stderr, "Failed to initialize SDL_TTF: %s\n", TTF_GetError());
        exit(1);
    }

    //Subscribe to certain messages
    gMessage_bus.subscribe(VSYNC_STATE, &msg_sub);
    gMessage_bus.subscribe(SET_VSYNC, &msg_sub);
    gMessage_bus.subscribe(SHOW_COLLISIONS, &msg_sub);
    gMessage_bus.subscribe(SHOW_FPS, &msg_sub);
    gMessage_bus.subscribe(SHOW_METRICS, &msg_sub);
    gMessage_bus.subscribe(SET_DARKNESS, &msg_sub);
    gMessage_bus.subscribe(SURFACES_ASSETS, &msg_sub);
    gMessage_bus.subscribe(FONTS_ASSETS, &msg_sub);
    gMessage_bus.subscribe(ENABLE_LIGHTING, &msg_sub);
    gMessage_bus.subscribe(SET_INTERNAL_RESOLUTION, &msg_sub);

    //debug collisions aren't shown by default
    collisions_shown = false;

    //Variables for lights
    darkness_level = 0;
    lights_quality = 5;

    //hash of default font
    default_font_normal = ST::hash_string(DEFAULT_FONT_NORMAL);
    default_font_small = ST::hash_string(DEFAULT_FONT_SMALL);

    //Initialize the rendering object
    ST::renderer_sdl::initialize(window, w_width, w_height);
    uint32_t screen_width_height = w_width | static_cast<uint32_t>(w_height << 16U);
    gMessage_bus.send_msg(new message(VIRTUAL_SCREEN_COORDINATES, screen_width_height));
}

/**
 * Consumes messages from the subscriber object.
 * Performs all drawing operations.
 * @param temp a pointer to the data of the current level.
 * @param fps the current frames per second.
 * @param gConsole a console object.
 */
void drawing_manager::update(const ST::level &temp, float fps, console &gConsole, ST::metrics metrics) {
    camera = temp.camera;
    handle_messages();

    ticks = SDL_GetTicks(); //CPU ticks since start
    ST::renderer_sdl::clear_screen(temp.background_color);

    draw_background(temp.background, temp.parallax_speed);

    std::vector<ST::entity> entities{};
    std::copy_if (temp.entities.begin(), temp.entities.end(), std::back_inserter(entities), [this](ST::entity e) {
        return is_onscreen(e);
    });

    draw_entities(entities);
    ST::renderer_sdl::draw_overlay(temp.overlay, static_cast<uint8_t>(ticks % temp.overlay_sprite_num), temp.overlay_sprite_num);
    draw_text_objects(temp.text_objects);

    if(lighting_enabled) {
        process_lights(temp.lights);
        draw_lights();
    }

    if (collisions_shown) {
        draw_collisions(entities);
        draw_coordinates(entities);
    }
    draw_fps(fps);

    if(!gConsole.is_open()) {
        draw_metrics(metrics, temp, entities);
    } else {
        draw_console(gConsole);
    }


    ST::renderer_sdl::present();
}

/**
 * Draws all visible text objects in the current level
 * @param objects a pointer to a vector of text_objects
 */
void drawing_manager::draw_text_objects(const std::vector<ST::text>& objects) const {
    for(auto& i : objects) {
        if (is_onscreen(i)) {
            ST::renderer_sdl::draw_text_lru_cached(i.font, i.text_string, i.x, i.y, i.color);
        }
    }
}

/**
 * Draws the fps counter on the screen.
 * @param fps The current fps.
 */
void drawing_manager::draw_fps(float fps) const {
    if(show_fps) {
        SDL_Color color_font = {255, 0, 255, 255};
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_normal, "fps:" + std::to_string(static_cast<int32_t>(fps)), 0, 50, color_font);
    }
}

/**
 *
 * @param metrics
 */
void drawing_manager::draw_metrics(ST::metrics metrics, const ST::level& level, const std::vector<ST::entity>& entities) const {
    if(metrics_shown) {
        SDL_Color color_font = {255, 0, 255, 255};
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "game_logic_time:" + std::to_string(metrics.game_logic_time), 0, 100, color_font);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "physics_time:" + std::to_string(metrics.physics_time), 0, 130, color_font);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "render_time:" + std::to_string(metrics.render_time), 0, 160, color_font);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "frame_time:" + std::to_string(metrics.frame_time), 0, 190, color_font);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "entities_on_screen:" + std::to_string(entities.size()), 0, 220, color_font);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "physics_objects:" + std::to_string(level.physics_objects_count), 0, 250, color_font);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "entities_total:" + std::to_string(level.entities.size()), 0, 280, color_font);
    }
}

/**
 * Draws the console window on the screen.
 * @param console A pointer to the console object.
 */
void drawing_manager::draw_console(console& console) const {
    ST::renderer_sdl::draw_rectangle_filled(0, 0, w_width, w_height/2, console.color);
    int pos = w_height/2;
    SDL_Color log_entry_color;
    for(auto i = console.entries.rbegin(); i != console.entries.rend(); ++i) {
        int pos_offset = pos - console.font_size + console.scroll_offset;
        if (pos_offset > 0 && pos_offset <= w_height / 2 + 50 - console.font_size * 2) {
            switch (i->type) {
                case ST::log_type::ERROR:
                    log_entry_color = console.color_error;
                    break;

                case ST::log_type::INFO:
                    log_entry_color = console.color_info;
                    break;

                default:
                    log_entry_color = console.color_success;
                    break;
            }
            ST::renderer_sdl::draw_text_cached_glyphs(default_font_normal, i->text, 0,
                                                      pos - console.font_size - 20 + console.scroll_offset, log_entry_color);
        }
        pos -= console.font_size + 5;
    }
    ST::renderer_sdl::draw_rectangle_filled(0, w_height/2 - console.font_size - 12, w_width, 3, console.color_text);
    int32_t cursor_draw_position;
    if (console.cursor_position == console.composition.size()) {
        cursor_draw_position = ST::renderer_sdl::draw_text_cached_glyphs(default_font_normal, "Input: " + console.composition, 0, w_height / 2, console.color_text);
    } else {
        std::string to_cursor = console.composition.substr(0, console.cursor_position);
        std::string after_cursor = console.composition.substr(console.cursor_position, INT_MAX);
        cursor_draw_position = ST::renderer_sdl::draw_text_cached_glyphs(default_font_normal, "Input: " + to_cursor, 0, w_height / 2, console.color_text);
        ST::renderer_sdl::draw_text_cached_glyphs(default_font_normal, after_cursor, cursor_draw_position, w_height / 2, console.color_text);
    }
    if (ticks - console.cursor_timer < 250 || console.cursor_timer == 0) {
        ST::renderer_sdl::draw_rectangle_filled(
                cursor_draw_position, w_height / 2 - 50 + 5, 3,
                console.font_size, console.color_text);
    }
    console.cursor_timer = (ticks - console.cursor_timer >= 500) * ticks +
                               (ticks - console.cursor_timer < 500) * console.cursor_timer;
}

/**
 * Processes the lightmap given a vector of light objects.
 * TODO: tests must be written for this method, but the algorithm calculating  the lighting is not yet finished.
 * @param lights A vector of <b>ST::light</b> objects.
 */
void drawing_manager::process_lights(const std::vector<ST::light>& lights){
    memset(lightmap, darkness_level, sizeof lightmap);
    for (const auto &light : lights) {
        int x = !light.is_static*(light.origin_x - camera.x) + light.is_static*light.origin_x;
        int y = !light.is_static*(light.origin_y - camera.y) + light.is_static*light.origin_y;
        double count = 0;
        double step = darkness_level/ static_cast<double>(light.radius);
        count = 0;
        int radius = light.radius;
        int intensity = light.intensity;
        if(x - radius - intensity > w_width || y - radius - intensity > w_height) {
            continue;
        }
        double step2 = 0;
        for(int i = y; i < y + radius + intensity; ++i){
            for(int j = x; j < x + radius + intensity; ++j){
                if(j > 0 && j < w_width && i > 0 && i < w_height) {
                    lightmap[j][i] = (uint8_t) light.brightness + static_cast<uint8_t>(count);
                }
                if(count + light.brightness < darkness_level && j > x + intensity) {
                    count += step;
                }
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i > y + intensity)
                step2 += step;
        }
        count = 0;
        step2 = 0;
        for(int i = y; i > y - radius - intensity; --i){
            for(int j = x; j > x - radius - intensity; --j){
                if(j > 0 && j < w_width && i > 0 && i < w_height) {
                    lightmap[j][i] = light.brightness + (uint8_t) count;
                }
                if(count + light.brightness < darkness_level && j < x - intensity) {
                    count += step;
                }
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i < y - intensity) {
                step2 += step;
            }
        }
        count = 0;
        step2 = 0;
        for(int i = y; i > y - radius - intensity; --i){
            for(int j = x; j < x + radius + intensity; ++j){
                if(j > 0 && j < w_width && i > 0 && i < w_height){
                    lightmap[j][i] = light.brightness + (uint8_t)count;
                }
                if(count + light.brightness < darkness_level && j > x + intensity) {
                    count += step;
                }
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i < y - intensity) {
                step2 += step;
            }
        }
        count = 0;
        step2 = 0;
        for(int i = y; i < y + radius + intensity; ++i){
            for(int j = x; j > x - radius - intensity; --j){
                if(j > 0 && j < w_width && i > 0 && i < w_height) {
                    lightmap[j][i] = light.brightness + (uint8_t) count;
                }
                if(count + light.brightness< darkness_level && j < x - intensity) {
                    count += step;
                }
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i > y + intensity) {
                step2 += step;
            }
        }
    }
}

/**
 * Draws the lightmap on the screen.
 */
void drawing_manager::draw_lights() const{
    //Losing a lot of fps here :)
    SDL_Rect tempRect = {0, 0, lights_quality, lights_quality};
    SDL_Color light_color;

    for(int i = 0; i <= w_width-lights_quality; i += lights_quality){
        int a = 1;
        tempRect.x = i;
        for(int j = 0; j <= w_height-lights_quality; j += lights_quality){
            if(lightmap[i][j] == lightmap[i][j+lights_quality] && j+lights_quality == w_height){
                tempRect.y = j - a*lights_quality;
                tempRect.h = (a+1)*lights_quality;
                light_color = {0, 0, 0, lightmap[i][j]};
                ST::renderer_sdl::draw_rectangle_filled(tempRect.x, tempRect.y, tempRect.w, tempRect.h, light_color);
            }
            else if(lightmap[i][j] == lightmap[i][j+lights_quality]){
                a++;
            }else{
                tempRect.h = a*lights_quality;
                tempRect.y = j - a*lights_quality;
                light_color = {0, 0, 0, lightmap[i][j]};
                ST::renderer_sdl::draw_rectangle_filled(tempRect.x, tempRect.y, tempRect.w, tempRect.h, light_color);
                a = 1;
            }
        }
    }
}

/**
 * Consume messages from the message bus and perform
 * the appropriate actions.
 */
void drawing_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        switch (temp->msg_name) {
            case SET_VSYNC: {
                auto arg = static_cast<bool>(temp->base_data0);
                if(arg){
                    ST::renderer_sdl::vsync_on();
                }else{
                    ST::renderer_sdl::vsync_off();
                }
                gMessage_bus.send_msg(new message(VSYNC_STATE, arg));
                break;
            }
            case SET_DARKNESS:
                set_darkness(static_cast<uint8_t>(temp->base_data0));
                break;
            case SHOW_COLLISIONS:
                collisions_shown = static_cast<bool>(temp->base_data0);
                break;
            case SHOW_FPS:
                show_fps = static_cast<bool>(temp->base_data0);
                break;
            case SHOW_METRICS:
                metrics_shown = static_cast<bool>(temp->base_data0);
                break;
            case ENABLE_LIGHTING:
                lighting_enabled = static_cast<bool>(temp->base_data0);
                break;
            case SURFACES_ASSETS: {
                auto surfaces = *static_cast<ska::bytell_hash_map<uint16_t, SDL_Surface *>**>(temp->get_data());
                ST::renderer_sdl::upload_surfaces(surfaces);
                break;
            }
            case FONTS_ASSETS: {
                auto fonts = *static_cast<ska::bytell_hash_map<uint16_t , TTF_Font *>**>(temp->get_data());
                ST::renderer_sdl::upload_fonts(fonts);
                break;
            }
            case SET_INTERNAL_RESOLUTION: {
                auto data = temp->base_data0;
                w_width = data & 0x0000ffffU;
                w_height = (data >> 16U) & 0x0000ffffU;
                gMessage_bus.send_msg(new message(VIRTUAL_SCREEN_COORDINATES, data));
                ST::renderer_sdl::set_resolution(w_width, w_height);
                break;
            }
        }
        delete temp;
        temp = msg_sub.get_next_message();
    }
}

/**
 * Set the level of darkness in the level.
 * @param arg The level of darkness where 255 is full black and 0 is fully lit.
 */
void drawing_manager::set_darkness(uint8_t arg){
    darkness_level = arg;
    memset(lightmap, arg, sizeof lightmap);
}

/**
 * Draws all visible entities on the screen.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_entities(const std::vector<ST::entity>& entities) const{
    uint32_t time = ticks >> 7U; //ticks/128
    for(auto& i : entities){
        int32_t camera_offset_x = (!i.is_static())*camera.x; //If entity isn't static add camera offset
        int32_t camera_offset_y = (camera_offset_x != 0)*camera.y;
        ST::renderer_sdl::draw_sprite_scaled(i.texture,
                                             i.x - camera_offset_x,
                                             i.y - camera_offset_y,
                                             time % i.sprite_num,
                                             i.animation,
                                             i.animation_num,
                                             i.sprite_num,
                                             i.tex_scale_x,
                                             i.tex_scale_y);
    }
}

/**
 * Tells if an entity is visible on the screen.
 * @param i The entity to check.
 * @return True if it is on screen and false otherwise.
 */
bool drawing_manager::is_onscreen(const ST::entity& i) const{
    return i.is_visible() &&
    (i.is_static() ||
    (i.x - camera.x + static_cast<int>(static_cast<float>(i.tex_w) * i.tex_scale_x) >= 0 &&
    i.x - camera.x <= w_width/* &&
    i.y + camera.y > 0 && //TODO: Not working quite right
    i.y - camera.y - static_cast<int>(static_cast<float>(i.tex_h) * i.tex_scale_y) <= w_height - static_cast<int>(static_cast<float>(i.tex_h) * i.tex_scale_y)*/));
}

/**
 * Tells if a text object is visible on the screen.
 * @param i The text object to check.
 * @return True if it is on screen and false otherwise.
 */
bool drawing_manager::is_onscreen(const ST::text& i) const {
    return i.is_visible && i.x < camera.x + w_width; //TODO: finish
}

/**
 * Draws the collision boxes for entities that are affected by physics.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_collisions(const std::vector<ST::entity>& entities) const{
    for(auto& i : entities) {
        int32_t x_offset = (!i.is_static())*camera.x;
        int32_t y_offset = (x_offset != 0)*camera.y;
        uint8_t b = (!i.is_affected_by_physics())*220;
        uint8_t r = (!b)*240;
        ST::renderer_sdl::draw_rectangle_filled(i.x - x_offset + i.get_col_x_offset(),
                                                i.y - y_offset + i.get_col_y_offset(), i.get_col_x(), i.get_col_y(),
                                                {r, 0, b, 100});
    }
}

/**
 * Draws the coordinates for entities that are affected by physics.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_coordinates(const std::vector<ST::entity>& entities) const{
    for(auto& i : entities) {
        if (i.is_affected_by_physics()) {
            int32_t x_offset = (!i.is_static())*camera.x;
            int32_t y_offset = (x_offset != 0)*camera.y;
            SDL_Color colour_text = {255, 255, 0, 255};
            ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "x: " + std::to_string(i.x), i.x - x_offset,
                                                      i.y - y_offset - i.tex_h, colour_text);
            ST::renderer_sdl::draw_text_cached_glyphs(default_font_small, "y: " + std::to_string(i.y), i.x - x_offset,
                                                      i.y - y_offset - i.tex_h + 30, colour_text);
        }
    }
}

/**
 * Draws all parallax background layers with the corresponding speed.
 * @param background an array of integers representing the background textures
 * @param parallax_speed an array of integers representing the parallax scrolling speed
 */
void drawing_manager::draw_background(const uint16_t background[PARALLAX_BG_LAYERS], const uint8_t parallax_speed[PARALLAX_BG_LAYERS]) const {
    for(uint8_t i = 0; i < PARALLAX_BG_LAYERS; i++) {
        ST::renderer_sdl::draw_background_parallax(background[i], (camera.x*(parallax_speed[i] << 3))/(w_width >> 1) % w_width);
    }
}

/**
 * Closes the drawing manager.
 * Quits the Font subsystem and destroys the renderer object.
 */
drawing_manager::~drawing_manager(){
    ST::renderer_sdl::close();
    TTF_Quit();
    singleton_initialized = false;
}
