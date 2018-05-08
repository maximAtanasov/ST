/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef INPUT_DEF
#define INPUT_DEF

#include <defs.hpp>
#include <input_manager/key_definitions.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>

///This object is responsible for taking input
/**
 *
 * Messages this subsystem listens to: <br>
 *
 * <b>VIRTUAL_SCREEN_COORDINATES</b> - Tell the input manager what the virtual screen resolution is.
 *
 * Message must contain: a pointer to a <b>std::tuple<int, int></b> containing the width and the height of the viewport. <br>
 *
 * <b>REAL_SCREEN_COORDINATES</b> - Tell the input manager what the actual screen resolution is.
 *
 * Message must contain: a pointer to a <b>std::tuple<int, int></b> containing the width and the height of the screen. <br>
 *
 * <b>START_TEXT_INPUT</b> - Starts taking all keyboard input and sends <b>TEXT_STREAM</b> messages.
 *
 * Message must contain: a <b>nullptr</b>.<br>
 *
 * <b>STOP_TEXT_INPUT</b> - Stops taking all keyboard input.
 *
 * Message must contain: a <b>nullptr</b>.<br>
 *
 * <b>CLEAR_TEXT_STREAM</b> - Clears the text buffer if text_input is true.
 *
 * Message must contain: a <b>nullptr</b>.<br>
 *
 * <b>REGISTER_KEY</b> - Registers the given key and sends input events (pressed, held, released) for it.
 *
 * Message must contain: a pointer to a <b>ST::key</b>. <br>
 *
 * <b>UNREGISTER_KEY</b> - Unregisters a previously registered key and stops sending input events for it.
 *
 * Message must contain: a pointer to a <b>ST::key</b>. <br>
 */
class input_manager{
	private:

        struct game_controls{
            const uint16_t keys = 512;
            uint8_t keyboardFramePrev[512]{};
            int mouseClicks[3]{};
            int mouseClicksFramePrev[3]{};
            const uint8_t* keyboard{};
			int mouseX_prev = 0, mouseY_prev = 0;
			int mouseX = 0, mouseY = 0;
			uint32_t mouse_scroll = 0;
        };

		int v_width = 1, v_height = 1;
		int r_width = 1, r_height = 1;
		float ratio_w = 1, ratio_h = 1;
		SDL_Event event{};
		game_controls controls{};
		message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
		subscriber msg_sub{};
		std::vector<ST::key> registered_keys;

        //this is where we save the text stream from our input manager
        std::string composition;
        bool text_input = false;

		bool keypress(ST::key);
		bool keyheld(ST::key);
		bool keyrelease(ST::key);
		void handle_messages();
        void take_input();
        static void update_task(void* mngr);

    public:
        input_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr);
        void close();
        void update();

};

//INLINED METHODS

/**
 * Starts the update_task() method using the task manager.
 */
inline void input_manager::update(){
	gTask_manager->start_task_lockfree(new ST::task(update_task, this, nullptr, -1));
}

#endif
