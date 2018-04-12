#include <ST_loaders/loaders.hpp>
#include <console/log.hpp>
#include <assets_manager/assets_manager.hpp>


int assets_manager::initialize(message_bus* msg_bus, task_manager* tsk_mngr){

    //set external dependencies
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;

    msg_sub = new subscriber();
    gMessage_bus->subscribe(LOAD_LIST, msg_sub);
    gMessage_bus->subscribe(UNLOAD_LIST, msg_sub);
    gMessage_bus->subscribe(LOAD_ASSET, msg_sub);
    gMessage_bus->subscribe(UNLOAD_ASSET, msg_sub);
    gMessage_bus->subscribe(LOAD_BINARY, msg_sub);

    //load the global assets
    load_assets_from_list("levels/assets_global.list");
    return 0;
}

//performs the update for the asset_manager on a task thread
void assets_manager::update_task(void* arg){
    auto self = (assets_manager*)arg;
    self->handle_messages();
}

void assets_manager::handle_messages(){
    message* temp = msg_sub->get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == LOAD_LIST){
            auto path = *(std::string*)temp->get_data();
            load_assets_from_list(path);
        }
        else if(temp->msg_name == UNLOAD_LIST){
            auto path = *(std::string*)temp->get_data();
            unload_assets_from_list(path);
        }else if(temp->msg_name == LOAD_ASSET){
            auto path = *(std::string*)temp->get_data();
            load_asset(path);
        }
        else if(temp->msg_name == UNLOAD_ASSET){
            auto path = *(std::string*)temp->get_data();
            unload_asset(path);
        }
        else if(temp->msg_name == LOAD_BINARY){
            auto path = *(std::string*)temp->get_data();
            load_assets_from_binary(path);
        }
        destroy_msg(temp);
        temp = msg_sub->get_next_message();
    }
}

//Loads assets contained within a binary
int assets_manager::load_assets_from_binary(const std::string& path) {
    ST::assets_named* assets1 = ST::unpack_binary(path);
    if(assets1 != nullptr){
        for(auto surface : assets1->surfaces){
            if(count[surface.first] > 0){
                count[surface.first]++;
                continue;
            }else{
                log(SUCCESS, "Unpacking " + surface.first);
                count[surface.first]++;
                std::hash<std::string> hash_f;
                size_t hashed = hash_f(surface.first);
                all_assets.surfaces[hashed] = surface.second;
            }
        }
        for(auto chunk : assets1->chunks){
            if(count[chunk.first] > 0){
                count[chunk.first]++;
                continue;
            }else{
                log(SUCCESS, "Unpacking " + chunk.first);
                count[chunk.first]++;
                std::hash<std::string> hash_f;
                size_t hashed = hash_f(chunk.first);
                all_assets.chunks.add(hashed, chunk.second);
            }
        }
        for(auto music : assets1->music){
            if(count[music.first] > 0){
                count[music.first]++;
                continue;
            }else{
                log(SUCCESS, "Unpacking " + music.first);
                count[music.first]++;
                std::hash<std::string> hash_f;
                size_t hashed = hash_f(music.first);
                all_assets.music[hashed] = music.second;
            }
        }
    }else{
        return -1;
    }
    return 0;
}

//loads an asset given a path to it
//handles .png, .wav, .mp3, .ttf
int assets_manager::load_asset(std::string path){
    //ignore a comment
    if(path.at(0) == '#'){
        return 0;
    }

    //check if the file is already loaded and increase the reference count
    if(count[path] > 0){
        count[path]++;
        return 0;
    }

    char extention[4];
    size_t length = path.length();
    extention[0] = path.c_str()[length - 3];
    extention[1] = path.c_str()[length - 2];
    extention[2] = path.c_str()[length - 1];
    extention[3] = 0;
    log(INFO, "Loading " + path);
    std::hash<std::string> hash_f;

    //Handle the different extentions - currently png, wav, mp3, ttf
    if(strcmp(extention, "png") == 0){
        SDL_Surface* temp1 = IMG_Load(path.c_str());
        if(temp1 != nullptr){
            for(Uint32 i = 0; i < path.size(); i++) {
                if (path.at(i) == '/') {
                    path.erase(0, i+1);
                }
            }
            size_t string_hash = hash_f(path);
            all_assets.surfaces[string_hash] = temp1;
            count[path]++;
        }else{
            log(ERROR, "File " + path + " not found");
        }
    }else if(strcmp(extention, "wav") == 0){
        Mix_Chunk* temp1 = Mix_LoadWAV(path.c_str());
        if (temp1 != nullptr){
            for(Uint32 i = 0; i < path.size(); i++) {
                if (path.at(i) == '/') {
                    path.erase(0, i+1);
                }
            }
            size_t string_hash = hash_f(path);
            all_assets.chunks.add(string_hash, temp1);
            count[path]++;
        }
        else{
            log(ERROR, "File " + path + " not found");
        }
    }else if(strcmp(extention, "ogg") == 0){
        Mix_Music* temp1 = Mix_LoadMUS(path.c_str());
        if (temp1 != nullptr) {
            for(Uint32 i = 0; i < path.size(); i++) {
                if (path.at(i) == '/') {
                    path.erase(0, i+1);
                }
            }
            size_t string_hash = hash_f(path);
            all_assets.music[string_hash] = temp1;
            count[path]++;
        }
        else {
            log(ERROR, "File " + path + " not found");
        }
    }else if(strcmp(extention, "bin") == 0){
        load_assets_from_binary(path);
    }else{ //if file is a font
        std::vector<std::string> result;
        std::istringstream iss(path);
        for(std::string path_; iss >> path;)
            result.push_back(path);
        int size;
        try{
            std::stringstream convert(result.at(1));
            convert >> size;
        }catch(const std::out_of_range& e){
			(void)e;
            log(ERROR, "Font size not specified!");
            return -1;
        }
        std::string font = result.at(0);
        TTF_Font* tempFont = TTF_OpenFont(font.c_str(), size);
        if(tempFont != nullptr){
            for(Uint32 i = 0; i < font.size(); i++) {
                if (font.at(i) == '/') {
                    font.erase(0, i+1);
                }
            }
            std::string font_and_size = font + result.at(1);
            all_assets.fonts[font_and_size] = tempFont;
            count[font_and_size]++;
        }else{
            log(ERROR, "File " + font + " not found!");
        }
    }
    return 0;
}

int assets_manager::load_assets_from_list(std::string path){
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
        log(ERROR, "File " + path + " not found");
        return -1;
    }
    gMessage_bus->send_msg(make_msg(ASSETS, make_data<ST::assets*>(&all_assets)));
    return 0;
}

int assets_manager::unload_assets_from_list(std::string path){
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
        log(ERROR, "File " + path + " not found");
        return -1;
    }
    gMessage_bus->send_msg(make_msg(ASSETS, make_data<ST::assets*>(&all_assets)));
    return 0;
}


//same as load_asset - except we unload it
int assets_manager::unload_asset(std::string path){
    //Ignore comments
    if(path.at(0) == '#'){
        return 0;
    }
    if(count[path] > 1){
        count[path]--;
        return 0;
    }
    char extention[4];
    size_t length = path.length();
    extention[0] = path.c_str()[length - 3];
    extention[1] = path.c_str()[length - 2];
    extention[2] = path.c_str()[length - 1];
    extention[3] = 0;
    log(INFO, "Unloading " + path);
    if(strcmp(extention, "png") == 0){
        std::hash<std::string> hash_f;
        size_t string_hash = hash_f(path);
        SDL_FreeSurface(all_assets.surfaces[string_hash]);
        all_assets.surfaces[string_hash] = nullptr;
        count[path]--;
    }else if(strcmp(extention, "wav") == 0){
        std::hash<std::string> hash_f;
        size_t string_hash = hash_f(path);
        Mix_FreeChunk(all_assets.chunks.get(string_hash));
        count[path]--;
    }else if(strcmp(extention, "ogg") == 0){
        std::hash<std::string> hash_f;
        size_t string_hash = hash_f(path);
        Mix_FreeMusic(all_assets.music[string_hash]);
        count[path]--;
    }else if(strcmp(extention, "bin") == 0){
        return 0;
    }else{ //if file is a font
        TTF_CloseFont(all_assets.fonts[path]);
        count[path]--;
    }
    return 0;
}

//unload all assets
void assets_manager::close(){
    handle_messages();
    delete msg_sub;
    for(auto& i : count){
        if(i.second > 0){
            i.second = 1;
            unload_asset(i.first);
        }
    }
}
