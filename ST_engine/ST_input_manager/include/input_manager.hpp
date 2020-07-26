/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef INPUT_DEF
#define INPUT_DEF

#include <key_definitions.hpp>
#include <key_event.hpp>
#include <message_bus.hpp>
#include <task_manager.hpp>
#include <SDL_events.h>
#include <SDL_haptic.h>

///This object is responsible for taking input
class input_manager{
	private:

		struct controller_buttons{
			bool a = false;
            bool b = false;
            bool x = false;
            bool y = false;
            bool left_stick = false;
            bool right_stick = false;
            bool dpad_up = false;
            bool dpad_down = false;
            bool dpad_right = false;
            bool dpad_left = false;
            bool left_shoulder = false;
            bool right_shoulder = false;
            bool start = false;
            bool select = false;
			int16_t left_trigger = 0;
            int16_t right_trigger = 0;
            int16_t right_stick_vertical = 0;
            int16_t right_stick_horizontal = 0;
            int16_t left_stick_vertical = 0;
            int16_t left_stick_horizontal = 0;
		};

        struct keyboard_and_mouse_controls{
            int32_t mouse_x = 0, mouse_y = 0;
            int16_t mouse_scroll = 0;
			bool *keyboard{};
            bool mouse_clicks[3]{};
        };

		int32_t v_width = 1, v_height = 1;
		int32_t r_width = 1, r_height = 1;

		float ratio_w = 1, ratio_h = 1;

        int16_t left_stick_horizontal_threshold = 0;
        int16_t left_stick_vertical_threshold = 0;

        int16_t right_stick_horizontal_threshold = 0;
        int16_t right_stick_vertical_threshold = 0;

        int16_t left_trigger_threshold = 0;
        int16_t right_trigger_threshold = 0;

    SDL_Event event{};

		message_bus& gMessage_bus;
        task_manager& gTask_manager;
		subscriber msg_sub{};
		std::vector<SDL_GameController*> controllers;
		std::vector<SDL_Haptic*> controllers_haptic;
		struct keyboard_and_mouse_controls controls{};
		struct keyboard_and_mouse_controls controls_prev_frame;
		struct controller_buttons controller_buttons;
		struct controller_buttons controller_button_prev_frame;

		//Store ST::keys and the amount of times the have been registered as values;
		ska::bytell_hash_map<ST::key, uint8_t> registered_keys;

        //this is where we save the text stream from our input manager
        std::string composition;
        bool text_input = false;

		bool key_event(ST::key, ST::key_event k_event) const;
		void handle_messages();
        void take_input();
        void take_controller_input();
        void take_mouse_input();
		static void update_task(void* mngr); //And private on linux

	public:

        input_manager(task_manager &gTask_manager, message_bus &gMessageBus);
        ~input_manager();
        void update();
};

//INLINED METHODS

/**
 * Starts the update_task() method using the task manager.
 */
inline void input_manager::update() {
#ifdef _MSC_VER
    input_manager::update_task(this);
#else
    gTask_manager.start_task_lockfree(new ST::task(update_task, this, nullptr));
#endif
}

#endif
