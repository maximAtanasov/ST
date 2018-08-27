#ifndef SLAVICTALES_GAME_MANAGER_MOCK_HPP
#define SLAVICTALES_GAME_MANAGER_MOCK_HPP

#include <vector>
#include <SDL2/SDL_atomic.h>
#include "../../../main/task_manager/task_manager.hpp"
#include "../../../main/message_bus/message_bus.hpp"
#include "../../../main/game_manager/level/level.hpp"
#include "../../../main/game_manager/level/level_data.hpp"

class game_manager{
private:

    //methods
    void handle_messages();
    void load_level(const std::string&){}
    void unload_level(const std::string&){}
    void start_level(const std::string&){}
    void reset_keys(){}
    void run_level_loop(){}

    ST::level_data* data = new ST::level_data();

public:

    bool fullscreen_status = false;
    bool vsync_flag = true;
    uint8_t volume_level = 0;

    game_manager(message_bus* msg_bus, task_manager* tsk_mngr){}
    ~game_manager() = default;
    std::string get_active_level() const {return "test_level";}

    ST::level_data* get_level_data() {
        get_level_data_calls++;
        return data;
    }

    bool key_pressed(size_t arg) const { return true;};
    bool key_held(size_t arg) const {return true;};
    bool key_released(size_t arg) const {return true;};
    int32_t get_mouse_x() const {return 100;};
    int32_t get_mouse_y() const {return 200;};
    void update() {};
    bool game_is_running() const {};
    ST::level get_level() const {};
    void center_camera_on_entity(uint64_t id){center_camera_on_entity_calls++;}

    //Variables to keep track of method calls
    uint8_t get_level_data_calls = 0;
    uint8_t center_camera_on_entity_calls = 0;
 };


#endif //SLAVICTALES_GAME_MANAGER_MOCK_HPP
