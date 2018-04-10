#ifndef INPUT_DEF
#define INPUT_DEF

#include <defs.hpp>
#include <manager.hpp>
#include <input_manager/key_definitions.hpp>

typedef struct game_controls{
    Uint8* keyboardFramePrev;
    int mouseClicks[3];
    int mouseClicksFramePrev[3];
    const Uint8* keyboard;
    int mouseX_prev, mouseY_prev;
    int mouseX, mouseY;
    int mouse_scroll;
} game_controls;

class input_manager : public virtual manager{
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
		void handle_messages() override;
        void take_input();
        static void update_task(void* mngr);

    public:
        input_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr) override;
        void close() override;
        void update() override;

};

#endif
