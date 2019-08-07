/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ASST_DEF
#define ASST_DEF

#include "assets.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <message_bus.hpp>
#include <task_manager.hpp>
#include <cstdint>
#include <ST_loaders/loaders.hpp>


///This object is responsible for loading/unloading assets.
class assets_manager{
    friend class asset_manager_test;
private:
        message_bus& gMessage_bus;
        task_manager* gTask_manager{};
        subscriber msg_sub{};
        ST::assets all_assets;
        ska::bytell_hash_map<std::string, uint16_t> count;
        int8_t load_asset(std::string path);
        int8_t unload_asset(std::string path);
        int8_t unload_assets_from_list(const std::string& path);
        int8_t load_assets_from_list(const std::string& path);
        int8_t load_assets_from_binary(const std::string& path);
        int8_t unload_assets_from_binary(const std::string& path);
        void handle_messages();
		void send_assets();

public:
        assets_manager(message_bus &gMessageBus, task_manager *tsk_mngr);
        ~assets_manager();
        void update();

    static void update_task(void* arg);
};

//INLINED METHODS

/**
 * will start the update task using the task manager.
 */
inline void assets_manager::update(){
    gTask_manager->start_task_lockfree(new ST::task(update_task, this, nullptr));
}

#endif
