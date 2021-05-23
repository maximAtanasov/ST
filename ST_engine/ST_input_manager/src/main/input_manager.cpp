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
input_manager::input_manager(task_manager &gTask_manager, message_bus &gMessageBus) : gMessage_bus(gMessageBus), gTask_manager(gTask_manager) {

    if(singleton_initialized){
        throw std::runtime_error("The input manager cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }

    if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0 ){
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("Could not initialize gamepad subsystem!")));
    }

    //Initialize controls
    int32_t length = 0;
    keyboard_sdl_raw = reinterpret_cast<bool*>(const_cast<uint8_t*>(SDL_GetKeyboardState(&length)));
    controls.buttons = static_cast<bool*>(malloc(static_cast<size_t>(length)));
    controls_prev_frame.buttons = static_cast<bool*>(malloc(static_cast<size_t>(length)));
    SDL_PollEvent(&event);

    //Subscribe to the messages we need
    gMessage_bus.subscribe(VIRTUAL_SCREEN_COORDINATES, &msg_sub);
    gMessage_bus.subscribe(REAL_SCREEN_COORDINATES, &msg_sub);
    gMessage_bus.subscribe(START_TEXT_INPUT, &msg_sub);
    gMessage_bus.subscribe(STOP_TEXT_INPUT, &msg_sub);
    gMessage_bus.subscribe(CLEAR_TEXT_STREAM, &msg_sub);
    gMessage_bus.subscribe(REGISTER_KEY, &msg_sub);
    gMessage_bus.subscribe(UNREGISTER_KEY, &msg_sub);
    gMessage_bus.subscribe(CONTROLLER_RUMBLE, &msg_sub);
    gMessage_bus.subscribe(SET_LEFT_JOYSTICK_HORIZONTAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_LEFT_JOYSTICK_VERTICAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_RIGHT_JOYSTICK_HORIZONTAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_RIGHT_JOYSTICK_VERTICAL_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_LEFT_TRIGGER_THRESHOLD, &msg_sub);
    gMessage_bus.subscribe(SET_RIGHT_TRIGGER_THRESHOLD, &msg_sub);
}

#ifndef _MSC_VER
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
 * Starts the update_task() method using the task manager.
 */
void input_manager::update() {
    gTask_manager.start_task_lockfree(new ST::task(update_task, this, nullptr));
}
#else
/**
 * Starts the update_task() method using the task manager.
 */
void input_manager::update() {
    handle_messages();
    take_input();
}
#endif

/**
 * Checks the state of the keyboard and any incoming events and sends appropriate messages.
 */
void input_manager::take_input(){
    while(SDL_PollEvent(&event) != 0){
        if(event.type == SDL_QUIT){
            gMessage_bus.send_msg(new message(END_GAME));
        }
        if(event.type == SDL_MOUSEWHEEL){
            controls.mouse_scroll += event.wheel.y;
            if(controls.mouse_scroll < 0){
                controls.mouse_scroll = 0;
            }
            gMessage_bus.send_msg(new message(MOUSE_SCROLL, controls.mouse_scroll));
        }
        if(event.type == SDL_TEXTINPUT){
            if(text_input) {
                composition += event.edit.text;
                //Drop the tilde when closing the console, otherwise it just stays there
                while(!composition.empty() && composition.at(composition.size()-1) == CONSOLE_TOGGLE_KEY){
                    composition.pop_back();
                }
                gMessage_bus.send_msg(new message(TEXT_STREAM, make_data(composition)));
            }
        }
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_BACKSPACE) {
                if (composition.length() > 0) {
                    composition.pop_back();
                    gMessage_bus.send_msg(new message(TEXT_STREAM, make_data(composition)));
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
            gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Found a controller: " + std::string(SDL_GameControllerName(controller)))));
            SDL_Haptic* haptic = SDL_HapticOpen(static_cast<int32_t>(controllers.size() - 1));
            if (haptic != nullptr) {
                if (SDL_HapticRumbleInit(haptic) < 0){
                    gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>(
                            "Unable to initialize rumble for controller " + std::string(SDL_GameControllerName(controller)))));
                }
                else {
                    gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>(
                            "The controller \"" + std::string(SDL_GameControllerName(controller)) +
                            "\" supports haptic feedback")));
                    controllers_haptic.emplace_back(haptic);
                }
            }
            else {
                gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>(
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
            gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Controller " + std::to_string(number+1) + " disconnected")));
        }
    }

    memcpy(controls_prev_frame.buttons, controls.buttons, 512);
    memcpy(controls.buttons, keyboard_sdl_raw, 512);
    take_controller_input();
    take_mouse_input();

    //check if any of the registered keys is pressed and send a message if so
    for(auto i : registered_keys){
        if (key_event(i.first, ST::key_event::PRESS)) {
            gMessage_bus.send_msg(new message(KEY_PRESSED, static_cast<uint8_t>(i.first)));
        }
        if (key_event(i.first, ST::key_event::HOLD)) {
            gMessage_bus.send_msg(new message(KEY_HELD, static_cast<uint8_t>(i.first)));
        }
        if (key_event(i.first, ST::key_event::RELEASE)) {
            gMessage_bus.send_msg(new message(KEY_RELEASED, static_cast<uint8_t>(i.first)));
        }
    }
}

void input_manager::take_mouse_input() {
    //Collect mouse input
    uint32_t mouse_state = SDL_GetMouseState(nullptr, nullptr);
    controls.buttons[1] = mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT);
    controls.buttons[2] = mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    controls.buttons[3] = mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT);

    //only send mouse coordinates if they change
    if(controls.mouse_x != controls_prev_frame.mouse_x){
        gMessage_bus.send_msg(new message(MOUSE_X, static_cast<uint32_t>(controls.mouse_x)));
        controls_prev_frame.mouse_x = controls.mouse_x;
    }
    if(controls.mouse_y != controls_prev_frame.mouse_y){
        gMessage_bus.send_msg(new message(MOUSE_Y, static_cast<uint32_t>(controls.mouse_y)));
        controls_prev_frame.mouse_y = controls.mouse_y;
    }
}

/**
 * Takes input from all available controllers and
 * send messages if any analog inputs are clicked.
 * Messages for controller buttons are sent in the take input method.
 */
void input_manager::take_controller_input(){
    controller_analog_inputs_prev_frame = controller_analog_inputs;
    for(SDL_GameController* c : controllers) {
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_A)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_A);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_B)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_B);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_X)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_X);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_Y)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_Y);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_DPAD_UP)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_UP);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_DPAD_DOWN)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_DPAD_LEFT)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_DPAD_RIGHT)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_LEFTSTICK)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSTICK);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_RIGHTSTICK)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_LEFTSHOULDER)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_RIGHTSHOULDER)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_START)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START);
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_SELECT)] = SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK);

        controller_analog_inputs.left_trigger = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        controller_analog_inputs.right_trigger = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        controller_analog_inputs.right_stick_horizontal = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_RIGHTX);
        controller_analog_inputs.right_stick_vertical = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_RIGHTY);
        controller_analog_inputs.left_stick_horizontal = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX);
        controller_analog_inputs.left_stick_vertical = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);

        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_LEFT_TRIGGER)] = controller_analog_inputs.left_trigger > 0;
        controls.buttons[static_cast<uint8_t>(ST::key::CONTROLLER_BUTTON_RIGHT_TRIGGER)] = controller_analog_inputs.right_trigger > 0;
    }
    //only send controller axis values if they change and the values exceed the thresholds

    //Branch-less checks
    controller_analog_inputs.left_trigger = (controller_analog_inputs.left_trigger >= left_trigger_threshold) * controller_analog_inputs.left_trigger; // NOLINT(cppcoreguidelines-narrowing-conversions)
    controller_analog_inputs.right_trigger = (controller_analog_inputs.right_trigger >= right_trigger_threshold) * controller_analog_inputs.right_trigger; // NOLINT(cppcoreguidelines-narrowing-conversions)

    if(controller_analog_inputs.left_trigger != controller_analog_inputs_prev_frame.left_trigger){
        gMessage_bus.send_msg(new message(LEFT_TRIGGER, controller_analog_inputs.left_trigger));
    }

    if(controller_analog_inputs.right_trigger != controller_analog_inputs_prev_frame.right_trigger){
        gMessage_bus.send_msg(new message(RIGHT_TRIGGER, controller_analog_inputs.right_trigger));
    }

    //Branch-less check
    controller_analog_inputs.left_stick_vertical = (controller_analog_inputs.left_stick_vertical > left_stick_vertical_threshold // NOLINT(cppcoreguidelines-narrowing-conversions)
            || controller_analog_inputs.left_stick_vertical < -left_stick_vertical_threshold) * controller_analog_inputs.left_stick_vertical;

    //Branch-less check
    controller_analog_inputs.left_stick_horizontal = (controller_analog_inputs.left_stick_horizontal > left_stick_horizontal_threshold // NOLINT(cppcoreguidelines-narrowing-conversions)
            || controller_analog_inputs.left_stick_horizontal < -left_stick_horizontal_threshold) * controller_analog_inputs.left_stick_horizontal;

    //Branch-less check
    controller_analog_inputs.right_stick_horizontal = (controller_analog_inputs.right_stick_horizontal > right_stick_horizontal_threshold // NOLINT(cppcoreguidelines-narrowing-conversions)
            || controller_analog_inputs.right_stick_horizontal < -right_stick_horizontal_threshold) * controller_analog_inputs.right_stick_horizontal;

    //Branch-less check
    controller_analog_inputs.right_stick_vertical = (controller_analog_inputs.right_stick_vertical > right_stick_vertical_threshold // NOLINT(cppcoreguidelines-narrowing-conversions)
            || controller_analog_inputs.right_stick_vertical < -right_stick_vertical_threshold) * controller_analog_inputs.right_stick_vertical;

    if(controller_analog_inputs.left_stick_vertical != controller_analog_inputs_prev_frame.left_stick_vertical){
        gMessage_bus.send_msg(new message(LEFT_STICK_VERTICAL, controller_analog_inputs.left_stick_vertical));
    }

    if(controller_analog_inputs.left_stick_horizontal != controller_analog_inputs_prev_frame.left_stick_horizontal){
        gMessage_bus.send_msg(new message(LEFT_STICK_HORIZONTAL, controller_analog_inputs.left_stick_horizontal));
    }

    if(controller_analog_inputs.right_stick_vertical != controller_analog_inputs_prev_frame.right_stick_vertical){
        gMessage_bus.send_msg(new message(RIGHT_STICK_VERTICAL, controller_analog_inputs.right_stick_vertical));
    }

    if(controller_analog_inputs.right_stick_horizontal != controller_analog_inputs_prev_frame.right_stick_horizontal){
        gMessage_bus.send_msg(new message(RIGHT_STICK_HORIZONTAL, controller_analog_inputs.right_stick_horizontal));
    }
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void input_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        switch (temp->msg_name) {
            case VIRTUAL_SCREEN_COORDINATES: {
                auto data = temp->base_data0;
                v_width = data & 0xffffU;
                v_height = (data >> 16U) & 0xffffU;
                ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
                ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
                break;
            }
            case REAL_SCREEN_COORDINATES: {
                auto data = temp->base_data0;
                r_width = data & 0xffffU;
                r_height = (data >> 16U) & 0xffffU;
                ratio_w = static_cast<float>(v_width) / static_cast<float>(r_width);
                ratio_h = static_cast<float>(v_height) / static_cast<float>(r_height);
                break;
            }
            case START_TEXT_INPUT:
                text_input = true;
                break;
            case STOP_TEXT_INPUT:
                text_input = false;
                break;
            case CLEAR_TEXT_STREAM:
                composition.clear();
                break;
            case REGISTER_KEY: {
                auto key_val = static_cast<ST::key>(temp->base_data0);
                ++registered_keys[key_val];
                break;
            }
            case UNREGISTER_KEY: {
                auto key_val = static_cast<ST::key>(temp->base_data0);
                if (registered_keys[key_val] > 1) {
                    --registered_keys[key_val];
                } else if(registered_keys[key_val] == 1) {
                    registered_keys.erase(key_val);
                }
                break;
            }
            case CONTROLLER_RUMBLE: {
                if(!controllers.empty() && !controllers_haptic.empty()){
                    float strength = *reinterpret_cast<float*>(&temp->base_data0);
                    uint16_t duration = temp->base_data1;
                    for(SDL_Haptic* haptic : controllers_haptic){
                        SDL_HapticRumblePlay(haptic, strength, duration);
                    }
                }
                break;
            }
            default: //Otherwise we process any threshold messages
                uint8_t left_stick_hor_check = (temp->msg_name == SET_LEFT_JOYSTICK_HORIZONTAL_THRESHOLD);
                this->left_stick_horizontal_threshold = left_stick_hor_check*static_cast<int16_t>(temp->base_data0) + // NOLINT(cppcoreguidelines-narrowing-conversions)
                        !left_stick_hor_check*this->left_stick_horizontal_threshold;

                uint8_t left_stick_ver_check = (temp->msg_name == SET_LEFT_JOYSTICK_VERTICAL_THRESHOLD);
                this->left_stick_vertical_threshold = left_stick_ver_check*static_cast<int16_t>(temp->base_data0) + // NOLINT(cppcoreguidelines-narrowing-conversions)
                        !left_stick_ver_check*this->left_stick_vertical_threshold;

                uint8_t right_stick_hor_check = (temp->msg_name == SET_RIGHT_JOYSTICK_HORIZONTAL_THRESHOLD);
                this->right_stick_horizontal_threshold = right_stick_hor_check*static_cast<int16_t>(temp->base_data0) + // NOLINT(cppcoreguidelines-narrowing-conversions)
                        !right_stick_hor_check*this->right_stick_horizontal_threshold;

                uint8_t right_stick_ver_check = (temp->msg_name == SET_RIGHT_JOYSTICK_VERTICAL_THRESHOLD);
                this->right_stick_vertical_threshold = right_stick_ver_check*static_cast<int16_t>(temp->base_data0) + // NOLINT(cppcoreguidelines-narrowing-conversions)
                        !right_stick_ver_check*this->right_stick_vertical_threshold;

                uint8_t right_trigger_check = (temp->msg_name == SET_RIGHT_TRIGGER_THRESHOLD);
                this->right_trigger_threshold = right_trigger_check*static_cast<int16_t>(temp->base_data0) + // NOLINT(cppcoreguidelines-narrowing-conversions)
                        !right_trigger_check*this->right_trigger_threshold;

                uint8_t left_trigger_check = (temp->msg_name == SET_LEFT_TRIGGER_THRESHOLD);
                this->left_trigger_threshold = left_trigger_check*static_cast<int16_t>(temp->base_data0) + // NOLINT(cppcoreguidelines-narrowing-conversions)
                        !left_trigger_check*this->left_trigger_threshold;
                break;
        }
        delete temp;
        temp = msg_sub.get_next_message();
    }
}

/**
 * Tells if a key's state corresponds to the k_event. Works by comparing it's
 * current state with the state from the previous frame.
 * @param arg an <b>ST::key</b> to test.
 * @param k_event an <b>ST::key_event</b> to test for.
 * @return True if event k_event has happened for key arg, false otherwise.
 */
bool input_manager::key_event(ST::key arg, ST::key_event k_event) const {
    auto key = static_cast<uint8_t>(arg);
    return (controls.buttons[key] ^ (k_event == ST::key_event::RELEASE))
    && (controls_prev_frame.buttons[key] ^ (k_event == ST::key_event::PRESS));
}