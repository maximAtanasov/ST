#ifndef ASST_DEF
#define ASST_DEF

#include "assets.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "../defs.hpp"
#include "../manager.hpp"
#include "../message_bus/message_bus.hpp"


class assets_manager : public virtual manager{
    friend class asset_manager_test;
private:

        message_bus* gMessage_bus{};
        task_manager* gTask_manager{};
        subscriber* msg_sub{};
        SDL_Renderer* renderer{};
        ST::assets all_assets;
        std::unordered_map<std::string, int> count;
        int load_asset(std::string path);
        int unload_asset(std::string path);
        int unload_assets_from_list(std::string path);
        int load_assets_from_list(std::string path);
        int load_assets_from_binary(const std::string& path);
        void handle_messages() override;
        static void update_task(void* arg);

    public:
        assets_manager() = default;
        int initialize(message_bus* msg_bus, task_manager* tsk_mngr) override;
        void close() override;
        void update() override;
};

#endif
