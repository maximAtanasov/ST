/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef ASST_DEF
#define ASST_DEF

#include <assets_manager/assets.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <defs.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task_manager.hpp>
#include <task_manager/task_allocator.hpp>
#include <cstdint>

///This object is responsible for loading/unloading assets.
/**
 *
 * Messages this subsystem listens to: <br>
 *
 * <b>LOAD_LIST</b> - Loads assets from a <b>.list</b> file. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the path to the file. <br>
 *
 * <b>UNLOAD_LIST</b> - Unloads assets from a <b>.list</b> file. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the path to the file. <br>
 *
 * <b>LOAD_ASSET</b> - Loads an asset given a path. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the path to the file. <br>
 *
 * <b>UNLOAD_ASSET</b> - Unload an asset given a path to it. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the path to the file. <br>
 *
 * <b>LOAD_BINARY</b> - Loads assets contained within a binary. <br>
 *
 * Message must contain: a pointer to a <b>std::string</b> containing the path to the file. <br>
 *
 */
class assets_manager{
    friend class asset_manager_test;
private:
        message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
        subscriber msg_sub{};
        ST::assets all_assets;
        ska::bytell_hash_map<std::string, uint16_t> count;
        int8_t load_asset(std::string path);
        int8_t unload_asset(std::string path);
        int8_t unload_assets_from_list(std::string path);
        int8_t load_assets_from_list(std::string path);
        int8_t load_assets_from_binary(const std::string& path);
        int8_t unload_assets_from_binary(const std::string& path);
        void handle_messages();
        static void update_task(void* arg);

    public:
        assets_manager(message_bus* msg_bus, task_manager* tsk_mngr);
        ~assets_manager();
        void update();
};

//INLINED METHODS

/**
 * will start the update task using the task manager.
 */
inline void assets_manager::update(){
    gTask_manager->start_task_lockfree(make_task(update_task, this, nullptr, -1));
}

#endif
