/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <drawing_manager/drawing_manager.hpp>
#include <console/log.hpp>
#include <defs.hpp>
#include <task_manager/task_allocator.hpp>

/**
 *
 * @param window A pointer to an SDL_Window to bind the renderer to.
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_manager.
 */
drawing_manager::drawing_manager(SDL_Window* window, message_bus* msg_bus, task_manager* tsk_mngr){
	if(TTF_Init() < 0){
		fprintf(stderr, "Failed to initialize SDL_TTF: %s\n", TTF_GetError());
		exit(1);
	}

    //set our external dependencies
    this->gMessage_bus = msg_bus;
    this->gTask_manager = tsk_mngr;

	//Subscribe to certain messages
	gMessage_bus->subscribe(VSYNC_ON, &msg_sub);
	gMessage_bus->subscribe(VSYNC_OFF, &msg_sub);
	gMessage_bus->subscribe(SHOW_COLLISIONS, &msg_sub);
    gMessage_bus->subscribe(SHOW_FPS, &msg_sub);
	gMessage_bus->subscribe(SET_DARKNESS, &msg_sub);
	gMessage_bus->subscribe(ASSETS, &msg_sub);
    gMessage_bus->subscribe(ENABLE_LIGHTING, &msg_sub);

	//debug collisions aren't shown by default
	collisions_shown = false;

	//Variables for lights
	darkness_level = 0;
    lights_quality = 4;

	//Initialize the rendering object
	gRenderer.initialize(window, w_width, w_height);
	gMessage_bus->send_msg(make_msg(VIRTUAL_SCREEN_COORDINATES, make_data(std::make_tuple(w_width, w_height))));
}

/**
 * Start a task to process the lightmap
 * @param arg a pointer to the drawing manager (a <b>this</b> pointer basically)
 */
void drawing_manager::process_lights_task(void *arg){
    auto self = static_cast<drawing_manager*>(arg);
    self->process_lights(self->lights_arg);
}

/**
 * Consumes messages from the subscriber object.
 * Performs all drawing operations.
 * @param temp a pointer to the data of the current level.
 * @param fps the current frames per second.
 * @param cnsl a console object.
 */
#ifdef __DEBUG
void drawing_manager::update(const ST::level_data& temp, double fps, const console& cnsl){
#elif defined(__RELEASE)
void drawing_manager::update(const ST::level_data& temp){
#endif
	Camera = temp.Camera;
	handle_messages();
    lights_arg = temp.lights;
    const bool lights_on = lighting_enabled;

    task_id id = nullptr;
    if(lights_on) {
        //start a task to pre-process lighting on a separate thread
        id = gTask_manager->start_task(make_task(process_lights_task, this, nullptr, -1));
    }

	ticks = SDL_GetTicks(); //CPU ticks since start
	gRenderer.clear_screen();
	gRenderer.draw_background(temp.background);
	draw_entities(temp.entities);

	gRenderer.draw_overlay(temp.overlay, ticks % temp.overlay_spriteNum, temp.overlay_spriteNum);
    draw_text_objects(temp.text_objects);

    //draw the lights when we are sure they are processed
    if(lights_on) {
        gTask_manager->wait_for_task(id);
        draw_lights();
    }

    //Draw debug info and the console in a debug build
    #ifdef __DEBUG
	if (collisions_shown) {
		draw_collisions(temp.entities);
		draw_coordinates(temp.entities);
	}
	draw_fps(fps);
	draw_console(cnsl);
    #endif

	gRenderer.present();
}


//TODO: Finish it
/**
 * Draws all visible text objects in the current level
 * @param objects a pointer to a vector of text_objects
 */
void drawing_manager::draw_text_objects(const std::vector<ST::text>& objects) {
    for(auto& i : objects) {
        if (i.is_visible()) {
            gRenderer.draw_text(i.get_font(), i.get_text_string(), i.get_x(), i.get_y() - i.get_font_size(), i.get_color(),
                                 i.get_font_size(), 2);
        }
    }
}

/**
 * Draws the fps counter on the screen.
 * @param fps The current fps.
 */
void drawing_manager::draw_fps(double fps){
    if(show_fps) {
        SDL_Color color_font = {255, 0, 255, 255};
        gRenderer.draw_text("OpenSans-Regular.ttf", "fps:" + std::to_string((int) fps), 0, 40, color_font, 40, 1);
    }
}

/**
 * Draws the console window on the screen.
 * @param cnsl A pointer to the console object.
 */
void drawing_manager::draw_console(const console& cnsl){
    if(cnsl.is_open()){
        gRenderer.draw_rectangle_filled(0, 0, w_width, w_height/2, cnsl.color);
        int pos = w_height/2;
        for(auto i = cnsl.entries.rbegin(); i != cnsl.entries.rend(); ++i) {
            if (pos - cnsl.font_size - 50 + cnsl.scroll_offset <= w_height / 2 - cnsl.font_size * 2) {
                gRenderer.draw_text("OpenSans-Regular.ttf", i->text, 0,
                                     pos - cnsl.font_size - 20 + cnsl.scroll_offset - 50, i->color,
                                     cnsl.font_size, -1);
            }
            pos -= cnsl.font_size + 5;
        }
        gRenderer.draw_rectangle_filled(0, w_height/2 - cnsl.font_size - 12, w_width, 3, cnsl.color_text);
        gRenderer.draw_text("OpenSans-Regular.ttf", "Command: " + cnsl.composition, 0, w_height/2-50, cnsl.color_text,
                             cnsl.font_size, -1);
    }
}

/**
 * Processes the lightmap given a vector of light objects.
 * TODO: tests must be written for this method, but the algorithm calculating  the lighting is not yet finished.
 * @param lights A vector of <b>ST::light</b> objects.
 */
void drawing_manager::process_lights(const std::vector<ST::light>& lights){
    #ifdef __DEBUG
    if(!lighting_enabled){
        return;
    }
    #endif
    for(int i = 0; i < w_width; i++){
        for(int j = 0; j < w_height; j++){
            lightmap[i][j] = darkness_level;
        }
    }
    for (const auto &light : lights) {
        int x = light.origin_x - Camera.x;
        int y = light.origin_y - Camera.y;
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
    for(int i = 0; i <= w_width-lights_quality; i += lights_quality){
        int a = 1;
        tempRect.x = i;
        for(int j = 0; j <= w_height-lights_quality; j += lights_quality){
            if(lightmap[i][j] == lightmap[i][j+lights_quality] && j+lights_quality == w_height){
                tempRect.y = j - a*lights_quality;
                tempRect.h = (a+1)*lights_quality;
                SDL_Color light_color = {0, 0, 0, lightmap[i][j]};
                gRenderer.draw_rectangle_filled(tempRect.x, tempRect.y, tempRect.w, tempRect.h, light_color);
            }
            else if(lightmap[i][j] == lightmap[i][j+lights_quality]){
                a++;
            }else{
                tempRect.h = a*lights_quality;
                tempRect.y = j - a*lights_quality;
                SDL_Color light_color = {0, 0, 0, lightmap[i][j]};
                gRenderer.draw_rectangle_filled(tempRect.x, tempRect.y, tempRect.w, tempRect.h, light_color);
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
        if(temp->msg_name == VSYNC_ON){
            gRenderer.vsync_on();
            gMessage_bus->send_msg(make_msg(VSYNC_IS_ON, nullptr));
        }
        else if(temp->msg_name == VSYNC_OFF){
            gRenderer.vsync_off();
            gMessage_bus->send_msg(make_msg(VSYNC_IS_OFF, nullptr));
        }
        else if(temp->msg_name == SET_DARKNESS){
            set_darkness(*(uint8_t*)temp->get_data());
        }
        else if(temp->msg_name == SHOW_COLLISIONS){
            auto arg = static_cast<bool*>(temp->get_data());
            if(*arg) {
                show_collisions();
            }
            else if(!(*arg)){
                hide_collisions();
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
        else if(temp->msg_name == ASSETS) {
            auto gAssets = static_cast<ST::assets**>(temp->get_data());
            ST::assets *temp_ast = *gAssets;
            asset_ptr = temp_ast;
            gRenderer.upload_surfaces(&asset_ptr->surfaces);
            gRenderer.upload_fonts(&asset_ptr->fonts);
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
    for(auto& i : entities){
        if(is_onscreen(i)){
            if(i.get_animation_num() == 0){
                if(i.is_static()){
                    gRenderer.draw_texture(i.get_texture(), i.get_x(), i.get_y());
				}
                else{
                    gRenderer.draw_texture(i.get_texture(), i.get_x() - Camera.x, i.get_y() - Camera.y);
				}
            }
            else{
                uint32_t time = ticks >> 7U; //ticks/128
                if(i.is_static()){
                    gRenderer.draw_sprite(i.get_texture(), i.get_x() , i.get_y(), time % i.get_sprite_num(), i.get_animation(), i.get_animation_num(), i.get_sprite_num());
                }else{
					gRenderer.draw_sprite(i.get_texture(), i.get_x() - Camera.x, i.get_y() - Camera.y , time % i.get_sprite_num(), i.get_animation(), i.get_animation_num(), i.get_sprite_num());
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
    if(!i.is_visible())
        return false;
    if(i.is_static())
        return true;
    return i.get_x() - Camera.x + i.get_tex_w() >= 0 && i.get_x() - Camera.x <= w_width
           && i.get_y()-Camera.y  > 0 && i.get_y()-Camera.y - i.get_tex_h() <= w_height;
}

/**
 * Draws the collision boxes for entities that are affected by physics.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_collisions(const std::vector<ST::entity>& entities) const{
    for(auto& i : entities)
        if(is_onscreen(i)){
            int Xoffset, Yoffset;
            if(i.is_static()){
                Xoffset = 0;
                Yoffset = 0;
            }else{
                Xoffset = Camera.x;
                Yoffset = Camera.y;
            }
            if(i.is_affected_by_physics()){
                SDL_Colour colour = {240, 0, 0, 100};
                gRenderer.draw_rectangle_filled(i.get_x() - Xoffset + i.get_col_x_offset(), i.get_y() - Yoffset + i.get_col_y_offset(), i.get_col_x(), i.get_col_y(), colour);
            }
            else{
                SDL_Colour colour2 = {0, 0, 220, 100};
                gRenderer.draw_rectangle_filled(i.get_x() - Xoffset + i.get_col_x_offset(), i.get_y() - Yoffset + i.get_col_y_offset(), i.get_col_x(), i.get_col_y(), colour2);
            }
        }
}

/**
 * Draws the coordinates for entities that are affected by physics.
 * @param entities A vector of entities in the current level.
 */
void drawing_manager::draw_coordinates(const std::vector<ST::entity>& entities){
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
                std::string tempX = "x: " + std::to_string(i.get_x());
                std::string tempY = "y: " + std::to_string(i.get_y());
                SDL_Colour colour_text = {255, 255, 0, 255};
                gRenderer.draw_text("OpenSans-Regular.ttf", tempX, i.get_x() - Xoffset,
                                     i.get_y() - Yoffset - i.get_tex_h(), colour_text, 25, 1);
                gRenderer.draw_text("OpenSans-Regular.ttf", tempY, i.get_x() - Xoffset,
                                     i.get_y() - Yoffset - i.get_tex_h() + 30, colour_text, 25, 1);
            }
        }
    }
}

/**
 * Enables the drawing of collision boxes.
 */
void drawing_manager::show_collisions(){
    collisions_shown = true;
}

/**
 * Disables the drawing of collision boxes.
 */
void drawing_manager::hide_collisions(){
    collisions_shown = false;
}

/**
 * Closes the drawing manager.
 * Quits the Font subsystem and destroys the renderer object.
 */
drawing_manager::~drawing_manager(){
	TTF_Quit();
}
