/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <input_manager.hpp>
#include <SDL.h>
#include <string>



static bool singleton_initialized = false;

input_manager::~input_manager() {
    singleton_initialized = false;
}

/**
 * initializes the input manager
 * @param msg_bus A pointer to the global message bus.
 * @param tsk_mngr A pointer to the global task_manager.
 */
input_manager::input_manager(message_bus* msg_bus, task_manager* tsk_mngr){

    if(singleton_initialized){
        throw std::runtime_error("The input manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    //SET OUR EXTERNAL DEPENDENCIES
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0 ){
        gMessage_bus->send_msg(new message(LOG_ERROR, make_data<std::string>("Could not initialize gamepad subsystem!")));
    }

    //Initialize controls
    int32_t length = 0;
    controls.keyboard = const_cast<uint8_t*>(SDL_GetKeyboardState(&length));
    controls_prev_frame.keyboard = static_cast<uint8_t*>(malloc(static_cast<size_t>(length)));
    SDL_PollEvent(&event);

    //Subscribe to the messages we need
	gMessage_bus->subscribe(VIRTUAL_SCREEN_COORDINATES, &msg_sub);
	gMessage_bus->subscribe(REAL_SCREEN_COORDINATES, &msg_sub);
    gMessage_bus->subscribe(START_TEXT_INPUT, &msg_sub);
    gMessage_bus->subscribe(STOP_TEXT_INPUT, &msg_sub);
    gMessage_bus->subscribe(CLEAR_TEXT_STREAM, &msg_sub);
    gMessage_bus->subscribe(REGISTER_KEY, &msg_sub);
    gMessage_bus->subscribe(UNREGISTER_KEY, &msg_sub);
    gMessage_bus->subscribe(CONTROLLER_RUMBLE, &msg_sub);
}

/**
 * Performs the update for the input_manager on a task thread.
 * @param arg pointer to an input_manager (a <b>this</b> pointer basically) as the
 * function must be static.
 */
void input_manager::update_task(void* mngr){
    auto self = static_cast<input_manager*>(mngr);
    self->handle_messages();
    self->take_input();
}

/**
 * Checks the state of the keyboard and any incoming events and sends appropriate messages.
 */
void input_manager::take_input(){

    memcpy(controls_prev_frame.keyboard, controls.keyboard, 512);
    take_controller_input();
    take_mouse_input();

    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT){
            gMessage_bus->send_msg(new message(END_GAME, nullptr));
        }
        if(event.type == SDL_MOUSEWHEEL){
            controls.mouse_scroll += event.wheel.y;
            if(controls.mouse_scroll < 0){
                controls.mouse_scroll = 0;
            }
            gMessage_bus->send_msg(new message(MOUSE_SCROLL, controls.mouse_scroll, nullptr));
        }
        if(event.type == SDL_TEXTINPUT){
            if(text_input) {
                composition += event.edit.text;
                //Drop the tilde when closing the console, otherwise it just stays there
                while(!composition.empty() && composition.at(composition.size()-1) == CONSOLE_TOGGLE_KEY){
                    composition.pop_back();
                }
                gMessage_bus->send_msg(new message(TEXT_STREAM, make_data(composition)));
            }
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE) {
                if (composition.length() > 0) {
                    composition.pop_back();
                    gMessage_bus->send_msg(new message(TEXT_STREAM, make_data(composition)));
                }
            }
        }
		if(event.type == SDL_MOUSEMOTION){
			SDL_GetMouseState(&controls.mouse_x, &controls.mouse_y);
			controls.mouse_x = static_cast<int>(static_cast<float>(controls.mouse_x)*ratio_w);
			controls.mouse_y = static_cast<int>(static_cast<float>(controls.mouse_y)*ratio_h);
		}
        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
            r_width = event.window.data1;
            r_height = event.window.data2;
            ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
            ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
        }
        if(event.cdevice.type == SDL_CONTROLLERDEVICEADDED){
            SDL_GameController* controller = SDL_GameControllerOpen(static_cast<int>(controllers.size()));
            controllers.emplace_back(controller);
			gMessage_bus->send_msg(new message(LOG_INFO, make_data<std::string>("Found a controller: " + std::string(SDL_GameControllerName(controller)))));
			SDL_Haptic* haptic = SDL_HapticOpen(static_cast<int32_t>(controllers.size() - 1));
			if (haptic != nullptr) {
				if (SDL_HapticRumbleInit(haptic) < 0){
					gMessage_bus->send_msg(new message(LOG_INFO, make_data<std::string>(
						"Unable to initialize rumble for controller " + std::string(SDL_GameControllerName(controller)))));
				}
				else {
                    gMessage_bus->send_msg(new message(LOG_INFO, make_data<std::string>(
                            "The controller \"" + std::string(SDL_GameControllerName(controller)) +
                            "\" supports haptic feedback")));
                    controllers_haptic.emplace_back(haptic);
				}
			}
			else {
				gMessage_bus->send_msg(new message(LOG_INFO, make_data<std::string>(
					"The controller \"" + std::string(SDL_GameControllerName(controller)) + 
					"\" does not support haptic feedback")));		
			}
        }
        if(event.cdevice.type == SDL_CONTROLLERDEVICEREMOVED){
            uint8_t number = 0;
            for(uint8_t i = 0; i < controllers.size() && i < 8; i++){
                if(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers.at(i))) == event.cdevice.which){
                    number = i;
                }
            }
            SDL_GameControllerClose(controllers.at(number));
            controllers.erase(controllers.begin() + number);
			if (number < controllers_haptic.size()) {
				SDL_HapticClose(controllers_haptic.at(number));
				controllers_haptic.erase(controllers_haptic.begin() + number);
			}
            gMessage_bus->send_msg(new message(LOG_INFO, make_data<std::string>("Controller " + std::to_string(number+1) + " disconnected")));
        }
    }

    //check if any of the registered keys is pressed and send a message if so
    for(auto i : registered_keys){
        if(i.second > 0) {
            if (keypress(i.first)) {
                gMessage_bus->send_msg(new message(KEY_PRESSED, static_cast<uint8_t>(i.first), nullptr));
            } else if (keyheld(i.first)) {
                gMessage_bus->send_msg(new message(KEY_HELD, static_cast<uint8_t>(i.first), nullptr));
            } else if (keyrelease(i.first)) {
                gMessage_bus->send_msg(new message(KEY_RELEASED, static_cast<uint8_t>(i.first), nullptr));
            }
        }
    }
}

void input_manager::take_mouse_input() {
    //Collect mouse input
    controls_prev_frame.mouse_clicks[0] = controls.mouse_clicks[0];
    controls_prev_frame.mouse_clicks[1] = controls.mouse_clicks[1];
    controls_prev_frame.mouse_clicks[2] = controls.mouse_clicks[2];
    controls.mouse_clicks[0] = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);
    controls.mouse_clicks[1] = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    controls.mouse_clicks[2] = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT);

    //only send mouse coordinates if they change
    if(controls.mouse_x != controls_prev_frame.mouse_x){
        gMessage_bus->send_msg(new message(MOUSE_X, static_cast<uint32_t>(controls.mouse_x), nullptr));
        controls_prev_frame.mouse_x = controls.mouse_x;
    }
    if(controls.mouse_y != controls_prev_frame.mouse_y){
        gMessage_bus->send_msg(new message(MOUSE_Y, static_cast<uint32_t>(controls.mouse_y), nullptr));
        controls_prev_frame.mouse_y = controls.mouse_y;
    }
}

/**
 * Takes input from all available controllers and
 * send messages if any analog inputs are clicked.
 * Messages for controller buttons are sent in the take input method.
 */
void input_manager::take_controller_input(){
    controller_button_prev_frame = controller_buttons;
    for(SDL_GameController* c : controllers) {
        controller_buttons.a = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_A);
        controller_buttons.b = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_B);
        controller_buttons.x = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_X);
        controller_buttons.y = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_Y);
        controller_buttons.dpad_up = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_UP);
        controller_buttons.dpad_down = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        controller_buttons.dpad_left = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        controller_buttons.dpad_right = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        controller_buttons.left_stick = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSTICK);
        controller_buttons.right_stick = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
        controller_buttons.left_shoulder = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        controller_buttons.right_shoulder = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        controller_buttons.start = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START);
        controller_buttons.select = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK);

        controller_buttons.left_trigger = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        controller_buttons.right_trigger = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        controller_buttons.right_stick_horizontal = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_RIGHTX);
        controller_buttons.right_stick_vertical = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_RIGHTY);
        controller_buttons.left_stick_horizontal = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX);
        controller_buttons.left_stick_vertical = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);
    }
    //only send controller axis values if they change
    if(controller_buttons.left_trigger != controller_button_prev_frame.left_trigger){
        gMessage_bus->send_msg(new message(LEFT_TRIGGER, controller_buttons.left_trigger, nullptr));
    }
    if(controller_buttons.right_trigger != controller_button_prev_frame.right_trigger){
        gMessage_bus->send_msg(new message(RIGHT_TRIGGER, controller_buttons.right_trigger, nullptr));
    }
    if(controller_buttons.left_stick_vertical != controller_button_prev_frame.left_stick_vertical){
        gMessage_bus->send_msg(new message(LEFT_STICK_VERTICAL, controller_buttons.left_stick_vertical, nullptr));
    }
    if(controller_buttons.left_stick_horizontal != controller_button_prev_frame.left_stick_horizontal){
        gMessage_bus->send_msg(new message(LEFT_STICK_HORIZONTAL, controller_buttons.left_stick_horizontal, nullptr));
    }
    if(controller_buttons.right_stick_vertical != controller_button_prev_frame.right_stick_vertical){
        gMessage_bus->send_msg(new message(RIGHT_STICK_VERTICAL, controller_buttons.right_stick_vertical, nullptr));
    }
    if(controller_buttons.right_stick_horizontal != controller_button_prev_frame.right_stick_horizontal){
        gMessage_bus->send_msg(new message(RIGHT_STICK_HORIZONTAL, controller_buttons.right_stick_horizontal, nullptr));
    }
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void input_manager::handle_messages(){
	message* temp = msg_sub.get_next_message();
	while(temp != nullptr){
		if(temp->msg_name == VIRTUAL_SCREEN_COORDINATES){
            auto data = temp->base_data0;
            v_width = data & 0x0000ffffU;
            v_height = (data >> 16U) & 0x0000ffffU;
            ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
            ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
		}else if(temp->msg_name == REAL_SCREEN_COORDINATES) {
            auto data = temp->base_data0;
            r_width = data & 0x0000ffffU;
            r_height = (data >> 16U) & 0x0000ffffU;
            ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
            ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
        }else if(temp->msg_name == START_TEXT_INPUT){
            text_input = true;
        }else if(temp->msg_name == STOP_TEXT_INPUT){
            text_input = false;
        }else if(temp->msg_name == CLEAR_TEXT_STREAM){
            composition.clear();
        }else if(temp->msg_name == REGISTER_KEY){
            auto key_val = static_cast<ST::key>(temp->base_data0);
            ++registered_keys[key_val];
        }else if(temp->msg_name == UNREGISTER_KEY) {
            auto key_val = static_cast<ST::key>(temp->base_data0);
            if (registered_keys[key_val] > 0) {
                --registered_keys[key_val];

            }
        }else if(temp->msg_name == CONTROLLER_RUMBLE){
            if(!controllers.empty() && !controllers_haptic.empty()){
                auto data = static_cast<std::tuple<float, uint32_t>*>(temp->get_data());
                for(SDL_Haptic* haptic : controllers_haptic){
                    SDL_HapticRumblePlay(haptic, std::get<0>(*data), std::get<1>(*data));
                }
            }
		}
		delete temp;
		temp = msg_sub.get_next_message();
	}
}

/**
 * Tells if a key is pressed or not. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to key for being pressed.
 * @return True if pressed, false otherwise.
 */
bool input_manager::keypress(ST::key arg) const{
    bool pressed = false;
    if(arg == ST::key::LEFT){
        pressed = controls.keyboard[SDL_SCANCODE_LEFT] && !controls_prev_frame.keyboard[SDL_SCANCODE_LEFT];
    }
    else if(arg == ST::key::RIGHT){
        pressed = controls.keyboard[SDL_SCANCODE_RIGHT] && !controls_prev_frame.keyboard[SDL_SCANCODE_RIGHT];
    }
    else if(arg == ST::key::UP){
        pressed = controls.keyboard[SDL_SCANCODE_UP] && !controls_prev_frame.keyboard[SDL_SCANCODE_UP];
    }
    else if(arg == ST::key::DOWN){
        pressed = controls.keyboard[SDL_SCANCODE_DOWN] && !controls_prev_frame.keyboard[SDL_SCANCODE_DOWN];
    }
    else if(arg == ST::key::A){
        pressed = controls.keyboard[SDL_SCANCODE_A] && !controls_prev_frame.keyboard[SDL_SCANCODE_A];
    }
    else if(arg == ST::key::B){
        pressed = controls.keyboard[SDL_SCANCODE_B] && !controls_prev_frame.keyboard[SDL_SCANCODE_B];
    }
    else if(arg == ST::key::C){
        pressed = controls.keyboard[SDL_SCANCODE_C] && !controls_prev_frame.keyboard[SDL_SCANCODE_C];
    }
    else if(arg == ST::key::D){
        pressed = controls.keyboard[SDL_SCANCODE_D] && !controls_prev_frame.keyboard[SDL_SCANCODE_D];
    }
    else if(arg == ST::key::E){
        pressed = controls.keyboard[SDL_SCANCODE_E] && !controls_prev_frame.keyboard[SDL_SCANCODE_E];
    }
    else if(arg == ST::key::F){
        pressed = controls.keyboard[SDL_SCANCODE_F] && !controls_prev_frame.keyboard[SDL_SCANCODE_F];
    }
    else if(arg == ST::key::G){
        pressed = controls.keyboard[SDL_SCANCODE_G] && !controls_prev_frame.keyboard[SDL_SCANCODE_G];
    }
    else if(arg == ST::key::H){
        pressed = controls.keyboard[SDL_SCANCODE_H] && !controls_prev_frame.keyboard[SDL_SCANCODE_H];
    }
    else if(arg == ST::key::I){
        pressed = controls.keyboard[SDL_SCANCODE_I] && !controls_prev_frame.keyboard[SDL_SCANCODE_I];
    }
    else if(arg == ST::key::J){
        pressed = controls.keyboard[SDL_SCANCODE_J] && !controls_prev_frame.keyboard[SDL_SCANCODE_G];
    }
    else if(arg == ST::key::K){
        pressed = controls.keyboard[SDL_SCANCODE_K] && !controls_prev_frame.keyboard[SDL_SCANCODE_K];
    }
    else if(arg == ST::key::L){
        pressed = controls.keyboard[SDL_SCANCODE_L] && !controls_prev_frame.keyboard[SDL_SCANCODE_L];
    }
    else if(arg == ST::key::M){
        pressed = controls.keyboard[SDL_SCANCODE_M] && !controls_prev_frame.keyboard[SDL_SCANCODE_M];
    }
    else if(arg == ST::key::N){
        pressed = controls.keyboard[SDL_SCANCODE_N] && !controls_prev_frame.keyboard[SDL_SCANCODE_N];
    }
    else if(arg == ST::key::O){
        pressed = controls.keyboard[SDL_SCANCODE_O] && !controls_prev_frame.keyboard[SDL_SCANCODE_O];
    }
    else if(arg == ST::key::P){
        pressed = controls.keyboard[SDL_SCANCODE_P] && !controls_prev_frame.keyboard[SDL_SCANCODE_P];
    }
    else if(arg == ST::key::Q){
        pressed = controls.keyboard[SDL_SCANCODE_Q] && !controls_prev_frame.keyboard[SDL_SCANCODE_Q];
    }
    else if(arg == ST::key::R){
        pressed = controls.keyboard[SDL_SCANCODE_R] && !controls_prev_frame.keyboard[SDL_SCANCODE_R];
    }
    else if(arg == ST::key::S){
        pressed = controls.keyboard[SDL_SCANCODE_S] && !controls_prev_frame.keyboard[SDL_SCANCODE_S];
    }
    else if(arg == ST::key::T){
        pressed = controls.keyboard[SDL_SCANCODE_T] && !controls_prev_frame.keyboard[SDL_SCANCODE_T];
    }
    else if(arg == ST::key::U){
        pressed = controls.keyboard[SDL_SCANCODE_U] && !controls_prev_frame.keyboard[SDL_SCANCODE_U];
    }
    else if(arg == ST::key::V){
        pressed = controls.keyboard[SDL_SCANCODE_V] && !controls_prev_frame.keyboard[SDL_SCANCODE_V];
    }
    else if(arg == ST::key::W){
        pressed = controls.keyboard[SDL_SCANCODE_W] && !controls_prev_frame.keyboard[SDL_SCANCODE_W];
    }
    else if(arg == ST::key::X){
        pressed = controls.keyboard[SDL_SCANCODE_X] && !controls_prev_frame.keyboard[SDL_SCANCODE_X];
    }
    else if(arg == ST::key::Y){
        pressed = controls.keyboard[SDL_SCANCODE_Y] && !controls_prev_frame.keyboard[SDL_SCANCODE_Y];
    }
    else if(arg == ST::key::Z){
        pressed = controls.keyboard[SDL_SCANCODE_Z] && !controls_prev_frame.keyboard[SDL_SCANCODE_Z];
    }
    else if(arg == ST::key::ONE){
        pressed = controls.keyboard[SDL_SCANCODE_1] && !controls_prev_frame.keyboard[SDL_SCANCODE_1];
    }
    else if(arg == ST::key::TWO){
        pressed = controls.keyboard[SDL_SCANCODE_2] && !controls_prev_frame.keyboard[SDL_SCANCODE_2];
    }
    else if(arg == ST::key::THREE){
        pressed = controls.keyboard[SDL_SCANCODE_3] && !controls_prev_frame.keyboard[SDL_SCANCODE_3];
    }
    else if(arg == ST::key::FOUR){
        pressed = controls.keyboard[SDL_SCANCODE_4] && !controls_prev_frame.keyboard[SDL_SCANCODE_4];
    }
    else if(arg == ST::key::FIVE){
        pressed = controls.keyboard[SDL_SCANCODE_5] && !controls_prev_frame.keyboard[SDL_SCANCODE_5];
    }
    else if(arg == ST::key::SIX){
        pressed = controls.keyboard[SDL_SCANCODE_6] && !controls_prev_frame.keyboard[SDL_SCANCODE_6];
    }
    else if(arg == ST::key::SEVEN){
        pressed = controls.keyboard[SDL_SCANCODE_7] && !controls_prev_frame.keyboard[SDL_SCANCODE_7];
    }
    else if(arg == ST::key::EIGHT){
        pressed = controls.keyboard[SDL_SCANCODE_8] && !controls_prev_frame.keyboard[SDL_SCANCODE_8];
    }
    else if(arg == ST::key::NINE){
        pressed = controls.keyboard[SDL_SCANCODE_9] && !controls_prev_frame.keyboard[SDL_SCANCODE_9];
    }
    else if(arg == ST::key::ZERO){
        pressed = controls.keyboard[SDL_SCANCODE_0] && !controls_prev_frame.keyboard[SDL_SCANCODE_0];
    }
    else if(arg == ST::key::ESCAPE){
        pressed = controls.keyboard[SDL_SCANCODE_ESCAPE] && !controls_prev_frame.keyboard[SDL_SCANCODE_ESCAPE];
    }
    else if(arg == ST::key::ENTER){
        pressed = controls.keyboard[SDL_SCANCODE_RETURN] && !controls_prev_frame.keyboard[SDL_SCANCODE_RETURN];
    }
    else if(arg == ST::key::SPACEBAR){
        pressed = controls.keyboard[SDL_SCANCODE_SPACE] && !controls_prev_frame.keyboard[SDL_SCANCODE_SPACE];
    }
    else if(arg == ST::key::TILDE){
        pressed = controls.keyboard[SDL_SCANCODE_GRAVE] && !controls_prev_frame.keyboard[SDL_SCANCODE_GRAVE];
    }
    else if(arg == ST::key::LSHIFT){
        pressed = controls.keyboard[SDL_SCANCODE_LSHIFT] && !controls_prev_frame.keyboard[SDL_SCANCODE_LSHIFT];
    }
    else if(arg == ST::key::BACKSPACE){
        pressed = controls.keyboard[SDL_SCANCODE_BACKSPACE] && !controls_prev_frame.keyboard[SDL_SCANCODE_BACKSPACE];
    }
    else if (arg == ST::key::DELETE) {
		pressed = controls.keyboard[SDL_SCANCODE_DELETE] && !controls_prev_frame.keyboard[SDL_SCANCODE_DELETE];
	}
    else if(arg == ST::key::BACKSLASH){
        pressed = controls.keyboard[SDL_SCANCODE_BACKSLASH] && !controls_prev_frame.keyboard[SDL_SCANCODE_BACKSLASH];
    }
    else if(arg == ST::key::CAPSLOCK){
        pressed = controls.keyboard[SDL_SCANCODE_CAPSLOCK] && !controls_prev_frame.keyboard[SDL_SCANCODE_CAPSLOCK];
    }
    else if(arg == ST::key::COMMA){
        pressed = controls.keyboard[SDL_SCANCODE_COMMA] && !controls_prev_frame.keyboard[SDL_SCANCODE_COMMA];
    }
    else if(arg == ST::key::EQUALS){
        pressed = controls.keyboard[SDL_SCANCODE_EQUALS] && !controls_prev_frame.keyboard[SDL_SCANCODE_EQUALS];
    }
    else if(arg == ST::key::LALT){
        pressed = controls.keyboard[SDL_SCANCODE_LALT] && !controls_prev_frame.keyboard[SDL_SCANCODE_LALT];
    }
    else if(arg == ST::key::LCTRL){
        pressed = controls.keyboard[SDL_SCANCODE_LCTRL] && !controls_prev_frame.keyboard[SDL_SCANCODE_LCTRL];
    }
    else if(arg == ST::key::LBRACKET){
        pressed = controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && !controls_prev_frame.keyboard[SDL_SCANCODE_LEFTBRACKET];
    }
    else if(arg == ST::key::RBRACKET){
        pressed = controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && !controls_prev_frame.keyboard[SDL_SCANCODE_RIGHTBRACKET];
    }
    else if(arg == ST::key::MINUS){
        pressed = controls.keyboard[SDL_SCANCODE_MINUS] && !controls_prev_frame.keyboard[SDL_SCANCODE_MINUS];
    }
    else if(arg == ST::key::RALT){
        pressed = controls.keyboard[SDL_SCANCODE_RALT] && !controls_prev_frame.keyboard[SDL_SCANCODE_RALT];
    }
    else if(arg == ST::key::RCTRL){
        pressed = controls.keyboard[SDL_SCANCODE_RCTRL] && !controls_prev_frame.keyboard[SDL_SCANCODE_RCTRL];
    }
    else if(arg == ST::key::SEMICOLON){
        pressed = controls.keyboard[SDL_SCANCODE_SEMICOLON] && !controls_prev_frame.keyboard[SDL_SCANCODE_SEMICOLON];
    }
    else if(arg == ST::key::SLASH){
        pressed = controls.keyboard[SDL_SCANCODE_SLASH] && !controls_prev_frame.keyboard[SDL_SCANCODE_SLASH];
    }
    else if(arg == ST::key::TAB){
        pressed = controls.keyboard[SDL_SCANCODE_TAB] && !controls_prev_frame.keyboard[SDL_SCANCODE_TAB];
    }
    else if(arg == ST::key::MOUSELEFT){
        pressed = !controls_prev_frame.mouse_clicks[0] && controls.mouse_clicks[0];
    }
    else if(arg == ST::key::MOUSEMIDDLE){
        pressed = !controls_prev_frame.mouse_clicks[1] && controls.mouse_clicks[1];
    }
    else if(arg == ST::key::MOUSERIGHT){
        pressed = !controls_prev_frame.mouse_clicks[2] && controls.mouse_clicks[2];
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_A){
        pressed = !controller_button_prev_frame.a && controller_buttons.a;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_B){
        pressed = !controller_button_prev_frame.b && controller_buttons.b;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_X){
        pressed = !controller_button_prev_frame.x && controller_buttons.x;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_Y){
        pressed = !controller_button_prev_frame.y && controller_buttons.y;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSTICK){
        pressed = !controller_button_prev_frame.left_stick && controller_buttons.left_stick;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSTICK){
        pressed = !controller_button_prev_frame.right_stick && controller_buttons.right_stick;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_UP){
        pressed = !controller_button_prev_frame.dpad_up && controller_buttons.dpad_up;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_DOWN){
        pressed = !controller_button_prev_frame.dpad_down && controller_buttons.dpad_down;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_LEFT){
        pressed = !controller_button_prev_frame.dpad_left && controller_buttons.dpad_left;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_RIGHT){
        pressed = !controller_button_prev_frame.dpad_right && controller_buttons.dpad_right;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSHOULDER){
        pressed = !controller_button_prev_frame.left_shoulder && controller_buttons.left_shoulder;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER){
        pressed = !controller_button_prev_frame.right_shoulder && controller_buttons.right_shoulder;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_SELECT){
        pressed = !controller_button_prev_frame.select && controller_buttons.select;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_START){
        pressed = !controller_button_prev_frame.start && controller_buttons.start;
    }
    return pressed;
}

/**
 * Tells if a key is held or not. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to key for being held.
 * @return True if held, false otherwise.
 */
bool input_manager::keyheld(ST::key arg) const{
    bool held = false;
    if(arg == ST::key::LEFT){
        held = controls.keyboard[SDL_SCANCODE_LEFT] && controls_prev_frame.keyboard[SDL_SCANCODE_LEFT];
    }
    else if(arg == ST::key::RIGHT){
        held = controls.keyboard[SDL_SCANCODE_RIGHT] && controls_prev_frame.keyboard[SDL_SCANCODE_RIGHT];
    }
    else if(arg == ST::key::UP){
        held = controls.keyboard[SDL_SCANCODE_UP] && controls_prev_frame.keyboard[SDL_SCANCODE_UP];
    }
    else if(arg == ST::key::DOWN){
        held = controls.keyboard[SDL_SCANCODE_DOWN] && controls_prev_frame.keyboard[SDL_SCANCODE_DOWN];
    }
    else if(arg == ST::key::A){
        held = controls.keyboard[SDL_SCANCODE_A] && controls_prev_frame.keyboard[SDL_SCANCODE_A];
    }
    else if(arg == ST::key::B){
        held = controls.keyboard[SDL_SCANCODE_B] && controls_prev_frame.keyboard[SDL_SCANCODE_B];
    }
    else if(arg == ST::key::C){
        held = controls.keyboard[SDL_SCANCODE_C] && controls_prev_frame.keyboard[SDL_SCANCODE_C];
    }
    else if(arg == ST::key::D){
        held = controls.keyboard[SDL_SCANCODE_D] && controls_prev_frame.keyboard[SDL_SCANCODE_D];
    }
    else if(arg == ST::key::E){
        held = controls.keyboard[SDL_SCANCODE_E] && controls_prev_frame.keyboard[SDL_SCANCODE_E];
    }
    else if(arg == ST::key::F){
        held = controls.keyboard[SDL_SCANCODE_F] && controls_prev_frame.keyboard[SDL_SCANCODE_F];
    }
    else if(arg == ST::key::G){
        held = controls.keyboard[SDL_SCANCODE_G] && controls_prev_frame.keyboard[SDL_SCANCODE_G];
    }
    else if(arg == ST::key::H){
        held = controls.keyboard[SDL_SCANCODE_H] && controls_prev_frame.keyboard[SDL_SCANCODE_H];
    }
    else if(arg == ST::key::I){
        held = controls.keyboard[SDL_SCANCODE_I] && controls_prev_frame.keyboard[SDL_SCANCODE_I];
    }
    else if(arg == ST::key::J){
        held = controls.keyboard[SDL_SCANCODE_J] && controls_prev_frame.keyboard[SDL_SCANCODE_G];
    }
    else if(arg == ST::key::K){
        held = controls.keyboard[SDL_SCANCODE_K] && controls_prev_frame.keyboard[SDL_SCANCODE_K];
    }
    else if(arg == ST::key::L){
        held = controls.keyboard[SDL_SCANCODE_L] && controls_prev_frame.keyboard[SDL_SCANCODE_L];
    }
    else if(arg == ST::key::M){
        held = controls.keyboard[SDL_SCANCODE_M] && controls_prev_frame.keyboard[SDL_SCANCODE_M];
    }
    else if(arg == ST::key::N){
        held = controls.keyboard[SDL_SCANCODE_N] && controls_prev_frame.keyboard[SDL_SCANCODE_N];
    }
    else if(arg == ST::key::O){
        held = controls.keyboard[SDL_SCANCODE_O] && controls_prev_frame.keyboard[SDL_SCANCODE_O];
    }
    else if(arg == ST::key::P){
        held = controls.keyboard[SDL_SCANCODE_P] && controls_prev_frame.keyboard[SDL_SCANCODE_P];
    }
    else if(arg == ST::key::Q){
        held = controls.keyboard[SDL_SCANCODE_Q] && controls_prev_frame.keyboard[SDL_SCANCODE_Q];
    }
    else if(arg == ST::key::R){
        held = controls.keyboard[SDL_SCANCODE_R] && controls_prev_frame.keyboard[SDL_SCANCODE_R];
    }
    else if(arg == ST::key::S){
        held = controls.keyboard[SDL_SCANCODE_S] && controls_prev_frame.keyboard[SDL_SCANCODE_S];
    }
    else if(arg == ST::key::T){
        held = controls.keyboard[SDL_SCANCODE_T] && controls_prev_frame.keyboard[SDL_SCANCODE_T];
    }
    else if(arg == ST::key::U){
        held = controls.keyboard[SDL_SCANCODE_U] && controls_prev_frame.keyboard[SDL_SCANCODE_U];
    }
    else if(arg == ST::key::V){
        held = controls.keyboard[SDL_SCANCODE_V] && controls_prev_frame.keyboard[SDL_SCANCODE_V];
    }
    else if(arg == ST::key::W){
        held = controls.keyboard[SDL_SCANCODE_W] && controls_prev_frame.keyboard[SDL_SCANCODE_W];
    }
    else if(arg == ST::key::X){
        held = controls.keyboard[SDL_SCANCODE_X] && controls_prev_frame.keyboard[SDL_SCANCODE_X];
    }
    else if(arg == ST::key::Y){
        held = controls.keyboard[SDL_SCANCODE_Y] && controls_prev_frame.keyboard[SDL_SCANCODE_Y];
    }
    else if(arg == ST::key::Z){
        held = controls.keyboard[SDL_SCANCODE_Z] && controls_prev_frame.keyboard[SDL_SCANCODE_Z];
    }
    else if(arg == ST::key::ONE){
        held = controls.keyboard[SDL_SCANCODE_1] && controls_prev_frame.keyboard[SDL_SCANCODE_1];
    }
    else if(arg == ST::key::TWO){
        held = controls.keyboard[SDL_SCANCODE_2] && controls_prev_frame.keyboard[SDL_SCANCODE_2];
    }
    else if(arg == ST::key::THREE){
        held = controls.keyboard[SDL_SCANCODE_3] && controls_prev_frame.keyboard[SDL_SCANCODE_3];
    }
    else if(arg == ST::key::FOUR){
        held = controls.keyboard[SDL_SCANCODE_4] && controls_prev_frame.keyboard[SDL_SCANCODE_4];
    }
    else if(arg == ST::key::FIVE){
        held = controls.keyboard[SDL_SCANCODE_5] && controls_prev_frame.keyboard[SDL_SCANCODE_5];
    }
    else if(arg == ST::key::SIX){
        held = controls.keyboard[SDL_SCANCODE_6] && controls_prev_frame.keyboard[SDL_SCANCODE_6];
    }
    else if(arg == ST::key::SEVEN){
        held = controls.keyboard[SDL_SCANCODE_7] && controls_prev_frame.keyboard[SDL_SCANCODE_7];
    }
    else if(arg == ST::key::EIGHT){
        held = controls.keyboard[SDL_SCANCODE_8] && controls_prev_frame.keyboard[SDL_SCANCODE_8];
    }
    else if(arg == ST::key::NINE){
        held = controls.keyboard[SDL_SCANCODE_9] && controls_prev_frame.keyboard[SDL_SCANCODE_9];
    }
    else if(arg == ST::key::ZERO){
        held = controls.keyboard[SDL_SCANCODE_0] && controls_prev_frame.keyboard[SDL_SCANCODE_0];
    }
    else if(arg == ST::key::ESCAPE){
        held = controls.keyboard[SDL_SCANCODE_ESCAPE] && controls_prev_frame.keyboard[SDL_SCANCODE_ESCAPE];
    }
    else if(arg == ST::key::ENTER){
        held = controls.keyboard[SDL_SCANCODE_RETURN] && controls_prev_frame.keyboard[SDL_SCANCODE_RETURN];
    }
    else if(arg == ST::key::SPACEBAR){
        held = controls.keyboard[SDL_SCANCODE_SPACE] && controls_prev_frame.keyboard[SDL_SCANCODE_SPACE];
    }
    else if(arg == ST::key::TILDE){
        held = controls.keyboard[SDL_SCANCODE_GRAVE] && controls_prev_frame.keyboard[SDL_SCANCODE_GRAVE];
    }
    else if(arg == ST::key::LSHIFT){
        held = controls.keyboard[SDL_SCANCODE_LSHIFT] && controls_prev_frame.keyboard[SDL_SCANCODE_LSHIFT];
    }
    else if(arg == ST::key::BACKSPACE){
        held = controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls_prev_frame.keyboard[SDL_SCANCODE_BACKSPACE];
    }
    else if (arg == ST::key::DELETE) {
		held = controls.keyboard[SDL_SCANCODE_DELETE] && controls_prev_frame.keyboard[SDL_SCANCODE_DELETE];
	}
    else if(arg == ST::key::BACKSLASH){
        held = controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls_prev_frame.keyboard[SDL_SCANCODE_BACKSLASH];
    }
    else if(arg == ST::key::CAPSLOCK){
        held = controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls_prev_frame.keyboard[SDL_SCANCODE_CAPSLOCK];
    }
    else if(arg == ST::key::COMMA){
        held = controls.keyboard[SDL_SCANCODE_COMMA] && controls_prev_frame.keyboard[SDL_SCANCODE_COMMA];
    }
    else if(arg == ST::key::EQUALS){
        held = controls.keyboard[SDL_SCANCODE_EQUALS] && controls_prev_frame.keyboard[SDL_SCANCODE_EQUALS];
    }
    else if(arg == ST::key::LALT){
        held = controls.keyboard[SDL_SCANCODE_LALT] && controls_prev_frame.keyboard[SDL_SCANCODE_LALT];
    }
    else if(arg == ST::key::LCTRL){
        held = controls.keyboard[SDL_SCANCODE_LCTRL] && controls_prev_frame.keyboard[SDL_SCANCODE_LCTRL];
    }
    else if(arg == ST::key::LBRACKET){
        held = controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls_prev_frame.keyboard[SDL_SCANCODE_LEFTBRACKET];
    }
    else if(arg == ST::key::RBRACKET){
        held = controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls_prev_frame.keyboard[SDL_SCANCODE_RIGHTBRACKET];
    }
    else if(arg == ST::key::MINUS){
        held = controls.keyboard[SDL_SCANCODE_MINUS] && controls_prev_frame.keyboard[SDL_SCANCODE_MINUS];
    }
    else if(arg == ST::key::RALT){
        held = controls.keyboard[SDL_SCANCODE_RALT] && controls_prev_frame.keyboard[SDL_SCANCODE_RALT];
    }
    else if(arg == ST::key::RCTRL){
        held = controls.keyboard[SDL_SCANCODE_RCTRL] && controls_prev_frame.keyboard[SDL_SCANCODE_RCTRL];
    }
    else if(arg == ST::key::SEMICOLON){
        held = controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls_prev_frame.keyboard[SDL_SCANCODE_SEMICOLON];
    }
    else if(arg == ST::key::SLASH){
        held = controls.keyboard[SDL_SCANCODE_SLASH] && controls_prev_frame.keyboard[SDL_SCANCODE_SLASH];
    }
    else if(arg == ST::key::TAB){
        held = controls.keyboard[SDL_SCANCODE_TAB] && controls_prev_frame.keyboard[SDL_SCANCODE_TAB];
    }
    else if(arg == ST::key::MOUSELEFT){
        held = controls_prev_frame.mouse_clicks[0] && controls.mouse_clicks[0];
    }
    else if(arg == ST::key::MOUSEMIDDLE){
        held = controls_prev_frame.mouse_clicks[1] && controls.mouse_clicks[1];
    }
    else if(arg == ST::key::MOUSERIGHT){
        held = controls_prev_frame.mouse_clicks[2] && controls.mouse_clicks[2];
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_A){
        held = controller_button_prev_frame.a && controller_buttons.a;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_B){
        held = controller_button_prev_frame.b && controller_buttons.b;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_X){
        held = controller_button_prev_frame.x && controller_buttons.x;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_Y){
        held = controller_button_prev_frame.y && controller_buttons.y;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSTICK){
        held = controller_button_prev_frame.left_stick && controller_buttons.left_stick;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSTICK){
        held = controller_button_prev_frame.right_stick && controller_buttons.right_stick;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_UP){
        held = controller_button_prev_frame.dpad_up && controller_buttons.dpad_up;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_DOWN){
        held = controller_button_prev_frame.dpad_down && controller_buttons.dpad_down;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_LEFT){
        held = controller_button_prev_frame.dpad_left && controller_buttons.dpad_left;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_RIGHT){
        held = controller_button_prev_frame.dpad_right && controller_buttons.dpad_right;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSHOULDER){
        held = controller_button_prev_frame.left_shoulder && controller_buttons.left_shoulder;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER){
        held = controller_button_prev_frame.right_shoulder && controller_buttons.right_shoulder;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_SELECT){
        held = controller_button_prev_frame.select && controller_buttons.select;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_START){
        held = controller_button_prev_frame.start && controller_buttons.start;
    }
    return held;
}

/**
 * Tells if a key is released or not. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to key for being released.
 * @return True if released, false otherwise.
 */
bool input_manager::keyrelease(ST::key arg) const{
    bool released = false;
    if(arg == ST::key::LEFT){
        released = !controls.keyboard[SDL_SCANCODE_LEFT] && controls_prev_frame.keyboard[SDL_SCANCODE_LEFT];
    }
    else if(arg == ST::key::RIGHT){
        released = !controls.keyboard[SDL_SCANCODE_RIGHT] && controls_prev_frame.keyboard[SDL_SCANCODE_RIGHT];
    }
    else if(arg == ST::key::UP){
        released = !controls.keyboard[SDL_SCANCODE_UP] && controls_prev_frame.keyboard[SDL_SCANCODE_UP];
    }
    else if(arg == ST::key::DOWN){
        released = !controls.keyboard[SDL_SCANCODE_DOWN] && controls_prev_frame.keyboard[SDL_SCANCODE_DOWN];
    }
    else if(arg == ST::key::A){
        released = !controls.keyboard[SDL_SCANCODE_A] && controls_prev_frame.keyboard[SDL_SCANCODE_A];
    }
    else if(arg == ST::key::B){
        released = !controls.keyboard[SDL_SCANCODE_B] && controls_prev_frame.keyboard[SDL_SCANCODE_B];
    }
    else if(arg == ST::key::C){
        released = !controls.keyboard[SDL_SCANCODE_C] && controls_prev_frame.keyboard[SDL_SCANCODE_C];
    }
    else if(arg == ST::key::D){
        released = !controls.keyboard[SDL_SCANCODE_D] && controls_prev_frame.keyboard[SDL_SCANCODE_D];
    }
    else if(arg == ST::key::E){
        released = !controls.keyboard[SDL_SCANCODE_E] && controls_prev_frame.keyboard[SDL_SCANCODE_E];
    }
    else if(arg == ST::key::F){
        released = !controls.keyboard[SDL_SCANCODE_F] && controls_prev_frame.keyboard[SDL_SCANCODE_F];
    }
    else if(arg == ST::key::G){
        released = !controls.keyboard[SDL_SCANCODE_G] && controls_prev_frame.keyboard[SDL_SCANCODE_G];
    }
    else if(arg == ST::key::H){
        released = !controls.keyboard[SDL_SCANCODE_H] && controls_prev_frame.keyboard[SDL_SCANCODE_H];
    }
    else if(arg == ST::key::I){
        released = !controls.keyboard[SDL_SCANCODE_I] && controls_prev_frame.keyboard[SDL_SCANCODE_I];
    }
    else if(arg == ST::key::J){
        released = !controls.keyboard[SDL_SCANCODE_J] && controls_prev_frame.keyboard[SDL_SCANCODE_G];
    }
    else if(arg == ST::key::K){
        released = !controls.keyboard[SDL_SCANCODE_K] && controls_prev_frame.keyboard[SDL_SCANCODE_K];
    }
    else if(arg == ST::key::L){
        released = !controls.keyboard[SDL_SCANCODE_L] && controls_prev_frame.keyboard[SDL_SCANCODE_L];
    }
    else if(arg == ST::key::M){
        released = !controls.keyboard[SDL_SCANCODE_M] && controls_prev_frame.keyboard[SDL_SCANCODE_M];
    }
    else if(arg == ST::key::N){
        released = !controls.keyboard[SDL_SCANCODE_N] && controls_prev_frame.keyboard[SDL_SCANCODE_N];
    }
    else if(arg == ST::key::O){
        released = !controls.keyboard[SDL_SCANCODE_O] && controls_prev_frame.keyboard[SDL_SCANCODE_O];
    }
    else if(arg == ST::key::P){
        released = !controls.keyboard[SDL_SCANCODE_P] && controls_prev_frame.keyboard[SDL_SCANCODE_P];
    }
    else if(arg == ST::key::Q){
        released = !controls.keyboard[SDL_SCANCODE_Q] && controls_prev_frame.keyboard[SDL_SCANCODE_Q];
    }
    else if(arg == ST::key::R){
        released = !controls.keyboard[SDL_SCANCODE_R] && controls_prev_frame.keyboard[SDL_SCANCODE_R];
    }
    else if(arg == ST::key::S){
        released = !controls.keyboard[SDL_SCANCODE_S] && controls_prev_frame.keyboard[SDL_SCANCODE_S];
    }
    else if(arg == ST::key::T){
        released = !controls.keyboard[SDL_SCANCODE_T] && controls_prev_frame.keyboard[SDL_SCANCODE_T];
    }
    else if(arg == ST::key::U){
        released = !controls.keyboard[SDL_SCANCODE_U] && controls_prev_frame.keyboard[SDL_SCANCODE_U];
    }
    else if(arg == ST::key::V){
        released = !controls.keyboard[SDL_SCANCODE_V] && controls_prev_frame.keyboard[SDL_SCANCODE_V];
    }
    else if(arg == ST::key::W){
        released = !controls.keyboard[SDL_SCANCODE_W] && controls_prev_frame.keyboard[SDL_SCANCODE_W];
    }
    else if(arg == ST::key::X){
        released = !controls.keyboard[SDL_SCANCODE_X] && controls_prev_frame.keyboard[SDL_SCANCODE_X];
    }
    else if(arg == ST::key::Y){
        released = !controls.keyboard[SDL_SCANCODE_Y] && controls_prev_frame.keyboard[SDL_SCANCODE_Y];
    }
    else if(arg == ST::key::Z){
        released = !controls.keyboard[SDL_SCANCODE_Z] && controls_prev_frame.keyboard[SDL_SCANCODE_Z];
    }
    else if(arg == ST::key::ONE){
        released = !controls.keyboard[SDL_SCANCODE_1] && controls_prev_frame.keyboard[SDL_SCANCODE_1];
    }
    else if(arg == ST::key::TWO){
        released = !controls.keyboard[SDL_SCANCODE_2] && controls_prev_frame.keyboard[SDL_SCANCODE_2];
    }
    else if(arg == ST::key::THREE){
        released = !controls.keyboard[SDL_SCANCODE_3] && controls_prev_frame.keyboard[SDL_SCANCODE_3];
    }
    else if(arg == ST::key::FOUR){
        released = !controls.keyboard[SDL_SCANCODE_4] && controls_prev_frame.keyboard[SDL_SCANCODE_4];
    }
    else if(arg == ST::key::FIVE){
        released = !controls.keyboard[SDL_SCANCODE_5] && controls_prev_frame.keyboard[SDL_SCANCODE_5];
    }
    else if(arg == ST::key::SIX){
        released = !controls.keyboard[SDL_SCANCODE_6] && controls_prev_frame.keyboard[SDL_SCANCODE_6];
    }
    else if(arg == ST::key::SEVEN){
        released = !controls.keyboard[SDL_SCANCODE_7] && controls_prev_frame.keyboard[SDL_SCANCODE_7];
    }
    else if(arg == ST::key::EIGHT){
        released = !controls.keyboard[SDL_SCANCODE_8] && controls_prev_frame.keyboard[SDL_SCANCODE_8];
    }
    else if(arg == ST::key::NINE){
        released = !controls.keyboard[SDL_SCANCODE_9] && controls_prev_frame.keyboard[SDL_SCANCODE_9];
    }
    else if(arg == ST::key::ZERO){
        released = !controls.keyboard[SDL_SCANCODE_0] && controls_prev_frame.keyboard[SDL_SCANCODE_0];
    }
    else if(arg == ST::key::ESCAPE){
        released = !controls.keyboard[SDL_SCANCODE_ESCAPE] && controls_prev_frame.keyboard[SDL_SCANCODE_ESCAPE];
    }
    else if(arg == ST::key::ENTER){
        released = !controls.keyboard[SDL_SCANCODE_RETURN] && controls_prev_frame.keyboard[SDL_SCANCODE_RETURN];
    }
    else if(arg == ST::key::SPACEBAR){
        released = !controls.keyboard[SDL_SCANCODE_SPACE] && controls_prev_frame.keyboard[SDL_SCANCODE_SPACE];
    }
    else if(arg == ST::key::TILDE){
        released = !controls.keyboard[SDL_SCANCODE_GRAVE] && controls_prev_frame.keyboard[SDL_SCANCODE_GRAVE];
    }
    else if(arg == ST::key::LSHIFT){
        released = !controls.keyboard[SDL_SCANCODE_LSHIFT] && controls_prev_frame.keyboard[SDL_SCANCODE_LSHIFT];
    }
    else if(arg == ST::key::BACKSPACE){
        released = !controls.keyboard[SDL_SCANCODE_BACKSPACE] && controls_prev_frame.keyboard[SDL_SCANCODE_BACKSPACE];
    }
    else if (arg == ST::key::DELETE) {
		released = !controls.keyboard[SDL_SCANCODE_DELETE] && controls_prev_frame.keyboard[SDL_SCANCODE_DELETE];
	}
    else if(arg == ST::key::BACKSLASH){
        released = !controls.keyboard[SDL_SCANCODE_BACKSLASH] && controls_prev_frame.keyboard[SDL_SCANCODE_BACKSLASH];
    }
    else if(arg == ST::key::CAPSLOCK){
        released = !controls.keyboard[SDL_SCANCODE_CAPSLOCK] && controls_prev_frame.keyboard[SDL_SCANCODE_CAPSLOCK];
    }
    else if(arg == ST::key::COMMA){
        released = !controls.keyboard[SDL_SCANCODE_COMMA] && controls_prev_frame.keyboard[SDL_SCANCODE_COMMA];
    }
    else if(arg == ST::key::EQUALS){
        released = !controls.keyboard[SDL_SCANCODE_EQUALS] && controls_prev_frame.keyboard[SDL_SCANCODE_EQUALS];
    }
    else if(arg == ST::key::LALT){
        released = !controls.keyboard[SDL_SCANCODE_LALT] && controls_prev_frame.keyboard[SDL_SCANCODE_LALT];
    }
    else if(arg == ST::key::LCTRL){
        released = !controls.keyboard[SDL_SCANCODE_LCTRL] && controls_prev_frame.keyboard[SDL_SCANCODE_LCTRL];
    }
    else if(arg == ST::key::LBRACKET){
        released = !controls.keyboard[SDL_SCANCODE_LEFTBRACKET] && controls_prev_frame.keyboard[SDL_SCANCODE_LEFTBRACKET];
    }
    else if(arg == ST::key::RBRACKET){
        released = !controls.keyboard[SDL_SCANCODE_RIGHTBRACKET] && controls_prev_frame.keyboard[SDL_SCANCODE_RIGHTBRACKET];
    }
    else if(arg == ST::key::MINUS){
        released = !controls.keyboard[SDL_SCANCODE_MINUS] && controls_prev_frame.keyboard[SDL_SCANCODE_MINUS];
    }
    else if(arg == ST::key::RALT){
        released = !controls.keyboard[SDL_SCANCODE_RALT] && controls_prev_frame.keyboard[SDL_SCANCODE_RALT];
    }
    else if(arg == ST::key::RCTRL){
        released = !controls.keyboard[SDL_SCANCODE_RCTRL] && controls_prev_frame.keyboard[SDL_SCANCODE_RCTRL];
    }
    else if(arg == ST::key::SEMICOLON){
        released = !controls.keyboard[SDL_SCANCODE_SEMICOLON] && controls_prev_frame.keyboard[SDL_SCANCODE_SEMICOLON];
    }
    else if(arg == ST::key::SLASH){
        released = !controls.keyboard[SDL_SCANCODE_SLASH] && controls_prev_frame.keyboard[SDL_SCANCODE_SLASH];
    }
    else if(arg == ST::key::TAB){
        released = !controls.keyboard[SDL_SCANCODE_TAB] && controls_prev_frame.keyboard[SDL_SCANCODE_TAB];
    }
    else if(arg == ST::key::MOUSELEFT){
        released = controls_prev_frame.mouse_clicks[0] && !controls.mouse_clicks[0];
    }
    else if(arg == ST::key::MOUSEMIDDLE){
        released = controls_prev_frame.mouse_clicks[1] && !controls.mouse_clicks[1];
    }
    else if(arg == ST::key::MOUSERIGHT){
        released = controls_prev_frame.mouse_clicks[2] && !controls.mouse_clicks[2];
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_A){
        released = controller_button_prev_frame.a && !controller_buttons.a;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_B){
        released = controller_button_prev_frame.b && !controller_buttons.b;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_X){
        released = controller_button_prev_frame.x && !controller_buttons.x;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_Y){
        released = controller_button_prev_frame.y && !controller_buttons.y;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSTICK){
        released = controller_button_prev_frame.left_stick && !controller_buttons.left_stick;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSTICK){
        released = controller_button_prev_frame.right_stick && !controller_buttons.right_stick;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_UP){
        released = controller_button_prev_frame.dpad_up && !controller_buttons.dpad_up;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_DOWN){
        released = controller_button_prev_frame.dpad_down && !controller_buttons.dpad_down;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_LEFT){
        released = controller_button_prev_frame.dpad_left && !controller_buttons.dpad_left;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_DPAD_RIGHT){
        released = controller_button_prev_frame.dpad_right && !controller_buttons.dpad_right;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_LEFTSHOULDER){
        released = controller_button_prev_frame.left_shoulder && !controller_buttons.left_shoulder;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER){
        released = controller_button_prev_frame.right_shoulder && !controller_buttons.right_shoulder;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_SELECT){
        released = controller_button_prev_frame.select && !controller_buttons.select;
    }
    else if(arg == ST::key::CONTROLLER_BUTTON_START){
        released = controller_button_prev_frame.start && !controller_buttons.start;
    }
    return released;
}