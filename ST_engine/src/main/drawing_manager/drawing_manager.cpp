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

#define DEFAULT_FONT_NORMAL "OpenSans-Regular.ttf 40"
#define DEFAULT_FONT_SMALL "OpenSans-Regular.ttf 40"

/**
 *
 * @param window A pointer to an SDL_Window to bind the renderer to.
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_manager.
 */
drawing_manager::drawing_manager(SDL_Window* window, message_bus* msg_bus){
	if(TTF_Init() < 0){
		fprintf(stderr, "Failed to initialize SDL_TTF: %s\n", TTF_GetError());
		exit(1);
	}

    //set our external dependencies
    gMessage_bus = msg_bus;

	//Subscribe to certain messages
	gMessage_bus->subscribe(VSYNC_STATE, &msg_sub);
	gMessage_bus->subscribe(SET_VSYNC, &msg_sub);
	gMessage_bus->subscribe(SHOW_COLLISIONS, &msg_sub);
    gMessage_bus->subscribe(SHOW_FPS, &msg_sub);
	gMessage_bus->subscribe(SET_DARKNESS, &msg_sub);
	gMessage_bus->subscribe(SURFACES_ASSETS, &msg_sub);
    gMessage_bus->subscribe(FONTS_ASSETS, &msg_sub);
    gMessage_bus->subscribe(ENABLE_LIGHTING, &msg_sub);
    gMessage_bus->subscribe(SET_INTERNAL_RESOLUTION, &msg_sub);

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
	gMessage_bus->send_msg(make_msg(VIRTUAL_SCREEN_COORDINATES, make_data(std::make_tuple(w_width, w_height))));
}

/**
 * Consumes messages from the subscriber object.
 * Performs all drawing operations.
 * @param temp a pointer to the data of the current level.
 * @param fps the current frames per second.
 * @param cnsl a console object.
 */
void drawing_manager::update(const ST::level& temp, double fps, console& cnsl){
	Camera = temp.Camera;
	handle_messages();

	ticks = SDL_GetTicks(); //CPU ticks since start
    ST::renderer_sdl::clear_screen(temp.background_color);
    ST::renderer_sdl::draw_background(temp.background);
	draw_entities(temp.entities);
    ST::renderer_sdl::draw_overlay(temp.overlay, static_cast<uint8_t>(ticks % temp.overlay_spriteNum), temp.overlay_spriteNum);
    draw_text_objects(temp.text_objects);
    //draw the lights when we are sure they are processed
    if(lighting_enabled) {
        process_lights(temp.lights);
        draw_lights();
    }
    //Draw debug info and the console in a debug build
	if (collisions_shown) {
		draw_collisions(temp.entities);
		draw_coordinates(temp.entities);
	}
	draw_fps(fps);
	draw_console(cnsl);

    ST::renderer_sdl::present();
}

/**
 * Draws all visible text objects in the current level
 * @param objects a pointer to a vector of text_objects
 */
void drawing_manager::draw_text_objects(const std::vector<ST::text>& objects) const{
    for(auto& i : objects) {
        if (is_onscreen(i)) {
            ST::renderer_sdl::draw_text(i.font, i.text_string, i.x, i.y, i.color, -1);
        }
    }
}

/**
 * Draws the fps counter on the screen.
 * @param fps The current fps.
 */
void drawing_manager::draw_fps(double fps) const{
    if(show_fps) {
        SDL_Color color_font = {255, 0, 255, 255};
        ST::renderer_sdl::draw_text(default_font_normal, "fps:" + std::to_string(static_cast<int32_t>(fps)), 0, 50, color_font, 1);
    }
}

/**
 * Draws the console window on the screen.
 * @param cnsl A pointer to the console object.
 */
void drawing_manager::draw_console(console& cnsl){
    if(cnsl.is_open()){
        ST::renderer_sdl::draw_rectangle_filled(0, 0, w_width, w_height/2, cnsl.color);
        int pos = w_height/2;
        for(auto i = cnsl.entries.rbegin(); i != cnsl.entries.rend(); ++i) {
            if (pos - cnsl.font_size + cnsl.scroll_offset <= w_height / 2 + 50 - cnsl.font_size * 2) {
                ST::renderer_sdl::draw_text(default_font_normal, i->text, 0,
                                     pos - cnsl.font_size - 20 + cnsl.scroll_offset, i->color, -1);
            }
            pos -= cnsl.font_size + 5;
        }
        ST::renderer_sdl::draw_rectangle_filled(0, w_height/2 - cnsl.font_size - 12, w_width, 3, cnsl.color_text);
		int32_t cursor_draw_position = 0;
		if (cnsl.cursor_position == cnsl.composition.size()) {
			cursor_draw_position = ST::renderer_sdl::draw_text(default_font_normal, "Command: " + cnsl.composition, 0, w_height / 2, cnsl.color_text, -1);
		}
		else {
			std::string to_cursor = cnsl.composition.substr(0, cnsl.cursor_position);
			std::string after_cursor = cnsl.composition.substr(cnsl.cursor_position, INT_MAX);
			cursor_draw_position = ST::renderer_sdl::draw_text(default_font_normal, "Command: " + to_cursor, 0, w_height / 2, cnsl.color_text, -1);
			ST::renderer_sdl::draw_text(default_font_normal, after_cursor, cursor_draw_position, w_height / 2, cnsl.color_text, -1);
		}

        if(ticks - cnsl.cursor_timer >= 1000) {
            cnsl.cursor_timer = ticks;
        }else if (ticks - cnsl.cursor_timer < 500) {
            ST::renderer_sdl::draw_rectangle_filled(
				cursor_draw_position, w_height / 2 - 50 + 5, 3,
                    cnsl.font_size, cnsl.color_text);
        }
    }
}

/**
 * Processes the lightmap given a vector of light objects.
 * TODO: tests must be written for this method, but the algorithm calculating  the lighting is not yet finished.
 * @param lights A vector of <b>ST::light</b> objects.
 */
void drawing_manager::process_lights(const std::vector<ST::light>& lights){
    if(!lighting_enabled){
        return;
    }
    for(int i = 0; i < w_width; i++){
        for(int j = 0; j < w_height; j++){
            lightmap[i][j] = darkness_level;
        }
    }
    for (const auto &light : lights) {
        int x = light.origin_x, y = light.origin_y;
        if(!light.is_static){
            x -= Camera.x;
            y -= Camera.y;
        }
        double count = 0;
        double step = darkness_level/ static_cast<double>(light.radius);
        count = 0;
        int radius = light.radius;
        int intensity = light.intensity;
        if(x - radius - intensity > w_width || y - radius - intensity > w_height)
            continue;
        double step2 = 0;
        for(int i = y; i < y + radius + intensity; i++){
            for(int j = x; j < x + radius + intensity; j++){
                if(j > 0 && j < w_width && i > 0 && i < w_height)
                    lightmap[j][i] = (uint8_t) light.brightness + (uint8_t)count;
                if(count + light.brightness < darkness_level && j > x + intensity)
                    count += step;
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i > y + intensity)
                step2 += step;
        }
        count = 0;
        step2 = 0;
        for(int i = y; i > y - radius - intensity; i--){
            for(int j = x; j > x - radius - intensity; j--){
                if(j > 0 && j < w_width && i > 0 && i < w_height)
                    lightmap[j][i] = light.brightness + (uint8_t)count;
                if(count + light.brightness < darkness_level && j < x - intensity)
                    count += step;
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i < y - intensity)
                step2 += step;
        }
        count = 0;
        step2 = 0;
        for(int i = y; i > y - radius - intensity; i--){
            for(int j = x; j < x + radius + intensity; j++){
                if(j > 0 && j < w_width && i > 0 && i < w_height)
                    lightmap[j][i] = light.brightness + (uint8_t)count;
                if(count + light.brightness < darkness_level && j > x + intensity)
                    count += step;
            }
            count = 0;
            count += step2;
            if(step2 + light.brightness < darkness_level && i < y - intensity)
                step2 += step;
        }
        count = 0;
        step2 = 0;
        for(int i = y; i < y + radius + intensity; i++){
            for(int j = x; j > x - radius - intensity; j--){
                if(j > 0 && j < w_width && i > 0 && i < w_height)
                    lightmap[j][i] = light.brightness + (uint8_t)count;
                if(count + light.brightness< darkness_level && j < x - intensity)
                    count += step;
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
        if(temp->msg_name == SET_VSYNC){
            auto arg = static_cast<bool*>(temp->get_data());
            if(*arg){
                ST::renderer_sdl::vsync_on();
            }else{
                ST::renderer_sdl::vsync_off();
            }
            gMessage_bus->send_msg(make_msg(VSYNC_STATE, make_data<>(*arg)));
        }
        else if(temp->msg_name == SET_DARKNESS){
            set_darkness(*(static_cast<uint8_t*>(temp->get_data())));
        }
        else if(temp->msg_name == SHOW_COLLISIONS){
            auto arg = static_cast<bool*>(temp->get_data());
            if(*arg) {
                collisions_shown = true;
            }
            else if(!(*arg)){
                collisions_shown = false;
            }
        }
        else if(temp->msg_name == SHOW_FPS){
            auto arg = static_cast<bool*>(temp->get_data());
            if(*arg) {
                show_fps = true;
            }
            else if(!(*arg)){
                show_fps = false;
            }
        }
        else if(temp->msg_name == ENABLE_LIGHTING){
            auto arg = static_cast<bool*>(temp->get_data());
            if(*arg) {
                lighting_enabled = true;
            }
            else if(!(*arg)){
                lighting_enabled = false;
            }
        }
        else if(temp->msg_name == SURFACES_ASSETS) {
            auto surfaces = *static_cast<ska::bytell_hash_map<uint16_t, SDL_Surface *>**>(temp->get_data());
            ST::renderer_sdl::upload_surfaces(surfaces);
        }
        else if(temp->msg_name == FONTS_ASSETS) {
            auto fonts = *static_cast<ska::bytell_hash_map<uint16_t , TTF_Font *>**>(temp->get_data());
            ST::renderer_sdl::upload_fonts(fonts);
        }
        else if(temp->msg_name == SET_INTERNAL_RESOLUTION) {
            auto res = *static_cast<std::tuple<int16_t, int16_t>*>(temp->get_data());
            gMessage_bus->send_msg(make_msg(VIRTUAL_SCREEN_COORDINATES, make_data(res)));
            ST::renderer_sdl::set_resolution(std::get<0>(res), std::get<1>(res));
            w_width = std::get<0>(res);
            w_height = std::get<1>(res);
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}

/**
 * Set the level of darkness in the level.
 * @param arg The level of darkness where 255 is full black and 0 is fully lit.
 */
void drawing_manager::set_darkness(uint8_t arg){
    darkness_level = arg;
    for(int i = 0; i < w_width; i++){
        for(int j = 0; j < w_height; j++)
            lightmap[i][j] = arg;
    }
}

/**
 * Draws all visible entities on the screen.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_entities(const std::vector<ST::entity>& entities) const{
    uint32_t time = ticks >> 7U; //ticks/128
    for(auto& i : entities){
        if(is_onscreen(i)){
            if(i.animation_num == 0){
                if(i.is_static()){
                    ST::renderer_sdl::draw_texture_scaled(i.texture, i.x, i.y, i.tex_scale_x, i.tex_scale_y);
				}
                else{
                    ST::renderer_sdl::draw_texture_scaled(i.texture, i.x - Camera.x, i.y - Camera.y, i.tex_scale_x, i.tex_scale_x);
				}
            }
            else{
                if(i.is_static()){
                    ST::renderer_sdl::draw_sprite_scaled(i.texture, i.x , i.y, time % i.sprite_num, i.animation, i.animation_num, i.sprite_num, i.tex_scale_x, i.tex_scale_y);
                }else{
                    ST::renderer_sdl::draw_sprite_scaled(i.texture, i.x - Camera.x, i.y - Camera.y , time % i.sprite_num, i.animation, i.animation_num, i.sprite_num, i.tex_scale_x, i.tex_scale_y);
				}
            }
        }
    }
}

/**
 * Tells if an entity is visible on the screen.
 * @param i The entity to check.
 * @return True if it is on screen and false otherwise.
 */
bool drawing_manager::is_onscreen(const ST::entity& i) const{
    if(!i.is_visible()) {
        return false;
    }
    else if(i.is_static()) {
        return true;
    }
    else {
        return i.x - Camera.x + i.tex_w >= 0 && i.x - Camera.x <= w_width
                && i.y - Camera.y > 0 && i.y - Camera.y - i.tex_h <= w_height;
    }
}

/**
 * Tells if a text object is visible on the screen.
 * @param i The text object to check.
 * @return True if it is on screen and false otherwise.
 */
bool drawing_manager::is_onscreen(const ST::text& i) const{
    return i.is_visible;//TODO: finish
}

/**
 * Draws the collision boxes for entities that are affected by physics.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_collisions(const std::vector<ST::entity>& entities) const{
    for(auto& i : entities) {
        if (is_onscreen(i)) {
            int Xoffset, Yoffset;
            if (i.is_static()) {
                Xoffset = 0;
                Yoffset = 0;
            } else {
                Xoffset = Camera.x;
                Yoffset = Camera.y;
            }
            if (i.is_affected_by_physics()) {
                SDL_Colour colour = {240, 0, 0, 100};
                ST::renderer_sdl::draw_rectangle_filled(i.x - Xoffset + i.get_col_x_offset(),
                                                i.y - Yoffset + i.get_col_y_offset(), i.get_col_x(), i.get_col_y(),
                                                colour);
            } else {
                SDL_Colour colour2 = {0, 0, 220, 100};
                ST::renderer_sdl::draw_rectangle_filled(i.x - Xoffset + i.get_col_x_offset(),
                                                i.y - Yoffset + i.get_col_y_offset(), i.get_col_x(), i.get_col_y(),
                                                colour2);
            }
        }
    }
}

/**
 * Draws the coordinates for entities that are affected by physics.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_coordinates(const std::vector<ST::entity>& entities) const{
    for(auto& i : entities) {
        if (is_onscreen(i)) {
            if (i.is_affected_by_physics()) {
                int Xoffset, Yoffset;
                if (i.is_static()) {
                    Xoffset = 0;
                    Yoffset = 0;
                } else {
                    Yoffset = Camera.y;
                    Xoffset = Camera.x;
                }
                std::string tempX = "x: " + std::to_string(i.x);
                std::string tempY = "y: " + std::to_string(i.y);
                SDL_Colour colour_text = {255, 255, 0, 255};
                ST::renderer_sdl::draw_text(default_font_small, tempX, i.x - Xoffset,
                                     i.y - Yoffset - i.tex_h, colour_text, 1);
                ST::renderer_sdl::draw_text(default_font_small, tempY, i.x - Xoffset,
                                     i.y - Yoffset - i.tex_h + 30, colour_text, 1);
            }
        }
    }
}

/**
 * Closes the drawing manager.
 * Quits the Font subsystem and destroys the renderer object.
 */
drawing_manager::~drawing_manager(){
    ST::renderer_sdl::close();
	TTF_Quit();
}
