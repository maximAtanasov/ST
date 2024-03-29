/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <ST_loaders/loaders.hpp>
#include <assets_manager.hpp>

static bool singleton_initialized = false;

/**
 * Initializes the asset_manager.
 * Loads all assets declared in the files
 * <b>assets_internal/assets_internal.list</b> and <b>game/levels/assets_global.list</b>.
 * @param msg_bus - A pointer to the global message bus.
 * @param tsk_mngr - A pointer to the global task manager.
 */
assets_manager::assets_manager(message_bus &gMessageBus, task_manager &gTask_manager) : gMessage_bus(gMessageBus),
                                                                                        gTask_manager(gTask_manager) {

    if (singleton_initialized) {
        throw std::runtime_error("The assets manager cannot be initialized more than once!");
    } else {
        singleton_initialized = true;
    }

    //subscribe to messages
    gMessage_bus.subscribe(LOAD_LIST, &msg_sub);
    gMessage_bus.subscribe(UNLOAD_LIST, &msg_sub);
    gMessage_bus.subscribe(LOAD_ASSET, &msg_sub);
    gMessage_bus.subscribe(UNLOAD_ASSET, &msg_sub);
    gMessage_bus.subscribe(LOAD_BINARY, &msg_sub);

    //load the internal engine assets and the global game assets
    load_assets_from_list("assets_internal/assets_internal.list");
    load_assets_from_list("game/levels/assets_global.list");
}

/**
 * Performs the update for the asset_manager on a task thread.
 * @param arg pointer to an assets_manager (a <b>this</b> pointer basically) as the
 * function must be static.
 */
void assets_manager::update_task(void *arg) {
    auto self = static_cast<assets_manager *>(arg);
    self->handle_messages();
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void assets_manager::handle_messages() {
    message *temp = msg_sub.get_next_message();
    while (temp != nullptr) {
        auto path = *static_cast<std::string *>(temp->get_data());
        switch (temp->msg_name) {
            case LOAD_LIST:
                load_assets_from_list(path);
                break;
            case UNLOAD_LIST:
                unload_assets_from_list(path);
                break;
            case LOAD_ASSET: {
                if (load_asset(path) == 0) {
                    send_assets();
                }
                break;
            }
            case UNLOAD_ASSET: {
                if (unload_asset(path) == 0) {
                    send_assets();
                }
                break;
            }
            case LOAD_BINARY:
                load_assets_from_binary(path);
                break;
        }
        delete temp;
        temp = msg_sub.get_next_message();
    }
}

/**
 * Loads assets contained within a binary.
 * @param path The path to the .bin (binary) file.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::load_assets_from_binary(const std::string &path) {
    ST::assets_named *assets1 = ST::unpack_binary(path);
    if (assets1 != nullptr) {
        for (const auto &surface: assets1->surfaces) {
            if (count[surface.first] == 0) {
                gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Unpacking " + surface.first)));
                uint16_t hashed = ST::hash_string(surface.first);
                all_assets.surfaces[hashed] = surface.second;
            }
            ++count[surface.first];
        }
        for (const auto &chunk: assets1->chunks) {
            if (count[chunk.first] == 0) {
                gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Unpacking " + chunk.first)));
                uint16_t hashed = ST::hash_string(chunk.first);
                all_assets.chunks[hashed] = chunk.second;
            }
            ++count[chunk.first];
        }
        for (const auto &music: assets1->music) {
            if (count[music.first] == 0) {
                gMessage_bus.send_msg(new message(LOG_SUCCESS, make_data<std::string>("Unpacking " + music.first)));
                count[music.first]++;
                uint16_t hashed = ST::hash_string(music.first);
                all_assets.music[hashed] = music.second;
            }
            ++count[music.first];
        }
        delete assets1;
    } else {
        return -1;
    }
    return 0;
}

/**
 * Unloads assets contained within a binary.
 * @param path The path to the .bin (binary) file.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::unload_assets_from_binary(const std::string &path) {
    ST::assets_named *assets1 = ST::unpack_binary(path);
    if (assets1 != nullptr) {
        for (const auto &surface: assets1->surfaces) {
            unload_asset(surface.first);
            SDL_FreeSurface(surface.second);
        }
        for (const auto &chunk: assets1->chunks) {
            unload_asset(chunk.first);
            Mix_FreeChunk(chunk.second);
        }
        for (const auto &music: assets1->music) {
            unload_asset(music.first);
            Mix_FreeMusic(music.second);
        }
        delete assets1;
    } else {
        return -1;
    }
    return 0;
}

void assets_manager::send_assets() {
    gMessage_bus.send_msg(new message(SURFACES_ASSETS, make_data(&all_assets.surfaces)));
    gMessage_bus.send_msg(new message(FONTS_ASSETS, make_data(&all_assets.fonts)));
    gMessage_bus.send_msg(new message(CHUNKS_ASSETS, make_data(&all_assets.chunks)));
    gMessage_bus.send_msg(new message(MUSIC_ASSETS, make_data(&all_assets.music)));
}

/**
 * Loads an asset given a path.
 * handles .png, .wav, .ogg, .ttf.
 * @param path the path to the file to load.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::load_asset(std::string path) {
    //ignore a comment
    if (path.at(0) == '#') {
        return 0;
    }

    //check if the file is already loaded and increase the reference count
    auto _asset_count = count.find(ST::trim_path(path));
    if (_asset_count != count.end() && _asset_count->second > 0) {
        ++_asset_count->second;
        return 0;
    } else if (_asset_count == count.end()) {
        count.emplace(ST::trim_path(path), 0);
    }

    ST::asset_file_type extension = ST::get_file_extension(path);
    //Handle the different extensions - currently png, webp, wav, mp3, ttf

    if (extension == ST::asset_file_type::BIN) {
        gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Loading from binary " + path)));
    } else {
        gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Loading " + path)));
    }

    if (extension == ST::asset_file_type::PNG || extension == ST::asset_file_type::WEBP) {
        SDL_Surface *temp1 = IMG_Load(path.c_str());
        if (temp1 != nullptr) {
            path = ST::trim_path(path);
            uint16_t string_hash = ST::hash_string(path);
            all_assets.surfaces[string_hash] = temp1;
            ++count.at(path);
        } else {
            gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
            return -1;
        }
    } else if (extension == ST::asset_file_type::WAV) {
        Mix_Chunk *temp1 = Mix_LoadWAV(path.c_str());
        if (temp1 != nullptr) {
            path = ST::trim_path(path);
            uint16_t string_hash = ST::hash_string(path);
            all_assets.chunks[string_hash] = temp1;
            ++count.at(path);
        } else {
            gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
            return -1;
        }
    } else if (extension == ST::asset_file_type::OGG) {
        Mix_Music *temp1 = Mix_LoadMUS(path.c_str());
        if (temp1 != nullptr) {
            path = ST::trim_path(path);
            uint16_t string_hash = ST::hash_string(path);
            all_assets.music[string_hash] = temp1;
            ++count.at(path);
        } else {
            gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
            return -1;
        }
    } else if (extension == ST::asset_file_type::BIN) {
        load_assets_from_binary(path);
    } else { //if file is a font
        std::vector<std::string> result;
        std::istringstream iss(path);
        for (std::string path_; iss >> path;) {
            result.push_back(path);
        }
        uint32_t size;
        try {
            std::stringstream convert(result.at(1));
            convert >> size;
        } catch (const std::out_of_range &e) {
            (void) e;
            gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("Font size not specified!")));
            return -1;
        }
        std::string font = result.at(0);
        //font sizes can only be positive
        TTF_Font *tempFont = TTF_OpenFont(font.c_str(), size); // NOLINT(cppcoreguidelines-narrowing-conversions)
        if (tempFont != nullptr) {
            font = ST::trim_path(font);
            std::string font_and_size = font + " " + result.at(1);
            all_assets.fonts[ST::hash_string(font_and_size)] = tempFont;
            ++count.at(font_and_size);
        } else {
            gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("File " + font + " not found!")));
            return -1;
        }
    }
    return 0;
}

/**
 * Loads assets from a .list file.
 * @param path The path to the .list file.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::load_assets_from_list(const std::string &path) {
    std::ifstream file;
    file.open(path.c_str());
    if (file.is_open()) {
        std::string temp;
        while (!file.eof()) {
            getline(file, temp);
            if (!temp.empty()) {
                load_asset(temp);
            }
        }
        file.close();
    } else {
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
        return -1;
    }
    send_assets();
    return 0;
}

/**
 * Unloads assets from a .list file.
 * @param path The path to the .list file.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::unload_assets_from_list(const std::string &path) {
    std::ifstream file;
    file.open(path.c_str());
    if (file.is_open()) {
        std::string temp;
        while (!file.eof()) {
            getline(file, temp);
            if (!temp.empty()) {
                //Ignore comments
                if (path.at(0) != '#') {
                    unload_asset(temp);
                }
            }
        }
        file.close();
    } else {
        gMessage_bus.send_msg(new message(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
        return -1;
    }
    send_assets();
    return 0;
}

/**
 * Unload an asset given a path to it.
 * @param path The path to the asset.
 * @return Always returns 0.
 */
int8_t assets_manager::unload_asset(std::string path) {
    auto _asset_count = count.find(ST::trim_path(path));
    if (_asset_count != count.end() && _asset_count->second > 1) {
        --_asset_count->second;
        return 0;
    } else if ((_asset_count == count.end() || _asset_count->second == 0) &&
               ST::get_file_extension(path) != ST::asset_file_type::BIN) {
        return -1;
    }

    ST::asset_file_type extension = ST::get_file_extension(path);
    gMessage_bus.send_msg(new message(LOG_INFO, make_data<std::string>("Unloading " + path)));

    if (extension == ST::asset_file_type::PNG || extension == ST::asset_file_type::WEBP) {
        path = ST::trim_path(path);
        uint16_t string_hash = ST::hash_string(path);
        SDL_FreeSurface(all_assets.surfaces[string_hash]);
        all_assets.surfaces[string_hash] = nullptr;
        --count.at(path);
    } else if (extension == ST::asset_file_type::WAV) {
        path = ST::trim_path(path);
        uint16_t string_hash = ST::hash_string(path);
        Mix_FreeChunk(all_assets.chunks[string_hash]);
        all_assets.chunks[string_hash] = nullptr;
        --count.at(path);
    } else if (extension == ST::asset_file_type::OGG) {
        path = ST::trim_path(path);
        uint16_t string_hash = ST::hash_string(path);
        Mix_FreeMusic(all_assets.music[string_hash]);
        all_assets.music[string_hash] = nullptr;
        --count.at(path);
    } else if (extension == ST::asset_file_type::BIN) {
        return unload_assets_from_binary(path);
    } else { //if file is a font
        path = ST::trim_path(path);
        TTF_CloseFont(all_assets.fonts[ST::hash_string(path)]);
        all_assets.fonts[ST::hash_string(path)] = nullptr;
        --count.at(path);
    }
    return 0;
}

/**
 * Closes the asset_manager.
 */
assets_manager::~assets_manager() {
    handle_messages();
    for (auto &i: count) {
        if (i.second > 0) {
            i.second = 1;
            unload_asset(i.first);
        }
    }
    singleton_initialized = false;
}

/**
 * will start the update task using the task manager.
 */
void assets_manager::update() {
    gTask_manager.start_task_lockfree(new ST::task(update_task, this, nullptr));
}
