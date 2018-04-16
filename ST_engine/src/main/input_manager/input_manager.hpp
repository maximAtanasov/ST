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

typedef struct game_controls{
    Uint8* keyboardFramePrev;
    int mouseClicks[3];
    int mouseClicksFramePrev[3];
    const Uint8* keyboard;
    int mouseX_prev, mouseY_prev;
    int mouseX, mouseY;
    int mouse_scroll;
} game_controls;

class input_manager{
	private:
		int v_width = 1, v_height = 1;
		int r_width = 1, r_height = 1;
		float ratio_w = 1, ratio_h = 1;
		SDL_Event event{};
		game_controls controls{};
		message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
		subscriber* msg_sub{};
		std::vector<key> registered_keys;
        task_id  update_lock{};

        //this is where we save the text stream from our input manager
        std::string composition;
        bool text_input = false;

		bool keypress(key);
		bool keyheld(key);
		bool keyrelease(key);
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

inline void input_manager::update(){
	gTask_manager->start_task_lockfree(new ST::task(update_task, this, nullptr, -1));
}

#endif
