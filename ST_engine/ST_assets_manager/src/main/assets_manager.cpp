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

/**
 * Initializes the asset_manager.
 * Loads all assets declared in the file <b>levels/assets_global.list</b>.
 * @param msg_bus - A pointer to the global message bus.
 * @param tsk_mngr - A pointer to the global task manager.
 */
assets_manager::assets_manager(message_bus* msg_bus, task_manager* tsk_mngr){
    //set external dependencies
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    //subscribe to messages
    gMessage_bus->subscribe(LOAD_LIST, &msg_sub);
    gMessage_bus->subscribe(UNLOAD_LIST, &msg_sub);
    gMessage_bus->subscribe(LOAD_ASSET, &msg_sub);
    gMessage_bus->subscribe(UNLOAD_ASSET, &msg_sub);
    gMessage_bus->subscribe(LOAD_BINARY, &msg_sub);

    //load the global assets
    load_assets_from_list("levels/assets_global.list");
}

/**
 * Performs the update for the asset_manager on a task thread.
 * @param arg pointer to an assets_manager (a <b>this</b> pointer basically) as the
 * function must be static.
 */
void assets_manager::update_task(void* arg){
    auto self = static_cast<assets_manager*>(arg);
    self->handle_messages();
}

/**
 * Retrieves messages from the subscriber object and
 * performs the appropriate actions.
 */
void assets_manager::handle_messages(){
    message* temp = msg_sub.get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == LOAD_LIST){
            auto path = *static_cast<std::string*>(temp->get_data());
            load_assets_from_list(path);
        }
        else if(temp->msg_name == UNLOAD_LIST){
            auto path = *static_cast<std::string*>(temp->get_data());
            unload_assets_from_list(path);
        }
        else if(temp->msg_name == LOAD_ASSET){
            auto path = *static_cast<std::string*>(temp->get_data());
			if (load_asset(path) == 0) {
				send_assets();
			}
        }
        else if(temp->msg_name == UNLOAD_ASSET){
            auto path = *static_cast<std::string*>(temp->get_data());
			if (unload_asset(path) == 0) {
				send_assets();
			}
        }
        else if(temp->msg_name == LOAD_BINARY){
            auto path = *static_cast<std::string*>(temp->get_data());
            load_assets_from_binary(path);
        }
        destroy_msg(temp);
        temp = msg_sub.get_next_message();
    }
}


/**
 * Loads assets contained within a binary.
 * @param path The path to the .bin (binary) file.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::load_assets_from_binary(const std::string& path) {
    ST::assets_named* assets1 = ST::unpack_binary(path);
    if(assets1 != nullptr){
        for(const auto &surface : assets1->surfaces){
            if(count[surface.first] > 0){
                count[surface.first]++;
                continue;
            }else{
                gMessage_bus->send_msg(make_msg(LOG_SUCCESS, make_data<std::string>("Unpacking " + surface.first)));
                count[surface.first]++;
                uint16_t hashed = hash_string(surface.first);
                all_assets.surfaces[hashed] = surface.second;
            }
        }
        for(const auto &chunk : assets1->chunks){
            if(count[chunk.first] > 0){
                count[chunk.first]++;
                continue;
            }else{
                gMessage_bus->send_msg(make_msg(LOG_SUCCESS, make_data<std::string>("Unpacking " + chunk.first)));
                count[chunk.first]++;
                std::hash<std::string> hash_f;
                size_t hashed = hash_f(chunk.first);
                all_assets.chunks[hashed] = chunk.second;
            }
        }
        for(const auto &music : assets1->music){
            if(count[music.first] > 0){
                count[music.first]++;
                continue;
            }else{
                gMessage_bus->send_msg(make_msg(LOG_SUCCESS, make_data<std::string>("Unpacking " + music.first)));
                count[music.first]++;
                std::hash<std::string> hash_f;
                size_t hashed = hash_f(music.first);
                all_assets.music[hashed] = music.second;
            }
        }
        delete assets1;
    }else{
        return -1;
    }
    return 0;
}

/**
 * Unloads assets contained within a binary.
 * @param path The path to the .bin (binary) file.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::unload_assets_from_binary(const std::string& path) {
    ST::assets_named* assets1 = ST::unpack_binary(path);
    if(assets1 != nullptr){
        for(const auto &surface : assets1->surfaces){
            unload_asset(surface.first);
            SDL_FreeSurface(surface.second);
        }
        for(const auto &chunk : assets1->chunks){
            unload_asset(chunk.first);
            Mix_FreeChunk(chunk.second);
        }
        for(const auto &music : assets1->music){
            unload_asset(music.first);
            Mix_FreeMusic(music.second);
        }
        delete assets1;
    }else{
        return -1;
    }
    return 0;
}

void assets_manager::send_assets() {
	gMessage_bus->send_msg(make_msg(SURFACES_ASSETS, make_data(&all_assets.surfaces)));
	gMessage_bus->send_msg(make_msg(FONTS_ASSETS, make_data(&all_assets.fonts)));
	gMessage_bus->send_msg(make_msg(CHUNKS_ASSETS, make_data(&all_assets.chunks)));
	gMessage_bus->send_msg(make_msg(MUSIC_ASSETS, make_data(&all_assets.music)));
}

/**
 * Loads an asset given a path.
 * handles .png, .wav, .ogg, .ttf.
 * @param path the path to the file to load.
 * @return -1 on failure or 0 on success.
 */
int8_t assets_manager::load_asset(std::string path){
    //ignore a comment
    if(path.at(0) == '#'){
        return 0;
    }

    //check if the file is already loaded and increase the reference count
    auto _asset_count = count.find(trim_path(path));
    if (_asset_count != count.end() && _asset_count->second > 0) {
        _asset_count->second += 1;
        return 0;
    } else if (_asset_count == count.end()) {
        count.emplace(trim_path(path), 0);
    }


    std::string extention;
    extention = ST::get_file_extension(path);
    if(extention == "bin"){
        gMessage_bus->send_msg(make_msg(LOG_INFO, make_data<std::string>("Loading from binary " + path)));
    }else {
        gMessage_bus->send_msg(make_msg(LOG_INFO, make_data<std::string>("Loading " + path)));
    }
    std::hash<std::string> hash_f;

    //Handle the different extentions - currently png, webp, wav, mp3, ttf
    if(extention == "png" || extention == "webp"){
        SDL_Surface* temp1 = IMG_Load(path.c_str());
        if(temp1 != nullptr){
            path = trim_path(path);
            uint16_t string_hash = hash_string(path);
            all_assets.surfaces[string_hash] = temp1;
            count.at(path) += 1;
        }else{
            gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
            return -1;
        }
    }else if(extention == "wav"){
        Mix_Chunk* temp1 = Mix_LoadWAV(path.c_str());
        if (temp1 != nullptr){
            path = trim_path(path);
            size_t string_hash = hash_f(path);
            all_assets.chunks[string_hash] = temp1;
            count.at(path) += 1;
        }
        else{
            gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
            return -1;
        }
    }else if(extention == "ogg"){
        Mix_Music* temp1 = Mix_LoadMUS(path.c_str());
        if (temp1 != nullptr) {
            path = trim_path(path);
            size_t string_hash = hash_f(path);
            all_assets.music[string_hash] = temp1;
            count.at(path) += 1;
        }
        else {
            gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
            return -1;
        }
    }else if(extention == "bin"){
        load_assets_from_binary(path);
    }else{ //if file is a font
        std::vector<std::string> result;
        std::istringstream iss(path);
        for(std::string path_; iss >> path;)
            result.push_back(path);
        uint32_t size;
        try{
            std::stringstream convert(result.at(1));
            convert >> size;
        }catch(const std::out_of_range& e){
			(void)e;
            gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("Font size not specified!")));
            return -1;
        }
        std::string font = result.at(0);
        TTF_Font* tempFont = TTF_OpenFont(font.c_str(), size);
        if(tempFont != nullptr){
            font = trim_path(font);
            std::string font_and_size = font + " " + result.at(1);
            all_assets.fonts[hash_f(font_and_size)] = tempFont;
            count.at(font_and_size) += 1;
        }else{
            gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + font + " not found!")));
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
int8_t assets_manager::load_assets_from_list(std::string path){
    std::ifstream file;
    file.open(path.c_str());
    if(file.is_open()){
        std::string temp;
        while(!file.eof()){
            getline(file, temp);
            if(!temp.empty()) {
                load_asset(temp);
            }
        }
        file.close();
    }
    else{
        gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
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
int8_t assets_manager::unload_assets_from_list(std::string path){
    std::ifstream file;
    file.open(path.c_str());
    if(file.is_open()){
        std::string temp;
        while(!file.eof()){
            getline(file, temp);
            if(!temp.empty()){
                unload_asset(temp);
            }
        }
        file.close();
    }
    else{
        gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
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
int8_t assets_manager::unload_asset(std::string path){
    //Ignore comments
    if(path.at(0) == '#'){
        return 0;
    }

    auto _asset_count = count.find(trim_path(path));
    if (_asset_count != count.end() && _asset_count->second > 1) {
        _asset_count->second -= 1;
        return 0;
    }else if((_asset_count == count.end() || _asset_count->second == 0) && ST::get_file_extension(path) != "bin"){
        return -1;
    }

    std::string extention = ST::get_file_extension(path);
    std::hash<std::string> hash_f;
    gMessage_bus->send_msg(make_msg(LOG_INFO, make_data<std::string>("Unloading " + path)));

    if(extention == "png" || extention == "webp"){
        path = trim_path(path);
        uint16_t string_hash = hash_string(path);
        SDL_FreeSurface(all_assets.surfaces[string_hash]);
        all_assets.surfaces[string_hash] = nullptr;
        count.at(path)--;
    }else if(extention == "wav"){
        path = trim_path(path);
        size_t string_hash = hash_f(path);
        Mix_FreeChunk(all_assets.chunks[string_hash]);
        all_assets.chunks[string_hash] = nullptr;
        count.at(path)--;
    }else if(extention == "ogg"){
        path = trim_path(path);
        size_t string_hash = hash_f(path);
        Mix_FreeMusic(all_assets.music[string_hash]);
        all_assets.music[string_hash] = nullptr;
        count.at(path)--;
    }else if(extention == "bin"){
        return unload_assets_from_binary(path);
    }else{ //if file is a font
        path = trim_path(path);
        TTF_CloseFont(all_assets.fonts[hash_f(path)]);
        all_assets.fonts[hash_f(path)] = nullptr;
        count.at(path)--;
    }
    return 0;
}

/**
 * Closes the asset_manager.
 * Consumes any leftover messages and unloads all assets.
 */
assets_manager::~assets_manager(){
    handle_messages();
    for(auto& i : count){
        if(i.second > 0){
            i.second = 1;
            unload_asset(i.first);
        }
    }
}
