/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <game_manager/lua_backend/lua_backend.hpp>
#include <game_manager/game_manager.hpp>
#include <console/log.hpp>

//local to the file, as lua bindings cannot be in a class
static message_bus*  gMessage_busLua;
static game_manager* gGame_managerLua;
static lua_backend* gLua_backendLua;

int lua_backend::initialize(message_bus* msg_bus, game_manager* game_mngr) {
    gLua_backendLua = this;
    gGame_managerLua = game_mngr;
    gMessage_busLua = msg_bus;
    gMessage_bus = msg_bus;
    L = luaL_newstate();
    if(L == nullptr){
        exit(1);
    }
    luaL_openlibs(L);

    luaL_dofile(L, "levels/general.lua");
    luaL_dofile(L, "levels/debug.lua");
    luaL_dofile(L, "levels/ui.lua");
    luaL_dofile(L, "levels/global_properties.lua");

    //register lua binding functions

    #ifdef __DEBUG
    lua_register(L, "logLua", logLua);
    lua_register(L, "showCollisions", showCollisionsLua);
    lua_register(L, "showFps", showFpsLua);
    #endif

    //General Functions
    lua_register(L, "switchRenderer", switchRendererLua);
    lua_register(L, "setFullscreen", setFullscreenLua);
    lua_register(L, "getFullscreenStatus", getFullscreenStatusLua);
    lua_register(L, "hashString", hashStringLua);
    lua_register(L, "delay", delayLua);
    lua_register(L, "use", useLua);
    lua_register(L, "vsyncOn", vsyncOnLua);
    lua_register(L, "vsyncOff", vsyncOffLua);
    lua_register(L, "getVsyncState", getVsyncStateLua);
    lua_register(L, "setBrightness", setBrightnessLua);
    lua_register(L, "startLevelLua", startLevelLua);
    lua_register(L, "showMouseCursor", showMouseCursorLua);
    lua_register(L, "hideMouseCursor", hideMouseCursorLua);
    lua_register(L, "endGame", endGameLua);
    lua_register(L, "centreCamera", centreCameraLua);
    lua_register(L, "setLevelSize", setLevelsizeLua);
    lua_register(L, "setLevelFloor", setLevelFloorLua);
    lua_register(L, "loadLevel", load_levelLua);
    lua_register(L, "unloadLevel", unload_levelLua);

    //PHYSICS
    lua_register(L, "setGravity", setGravityLua);
    lua_register(L, "pausePhysics", pausePhysicsLua);
    lua_register(L, "unpausePhysics", unpausePhysicsLua);

    //Drawing functions
    lua_register(L, "setBackground", setBackgroundLua);
    lua_register(L, "setOverlay", setOverlayLua);
    lua_register(L, "centreCamera", centreCameraLua);

    //Input functions
    lua_register(L, "getMouseX", getMouseXLua);
    lua_register(L, "getMouseY", getMouseYLua);
    lua_register(L, "keyHeld", keyHeldLua);
    lua_register(L, "keyPressed", keyPressedLua);
    lua_register(L, "keyReleased", keyReleasedLua);

    //Audio functions
    lua_register(L, "playSound", playSoundLua);
    lua_register(L, "playMusic", playMusicLua);
    lua_register(L, "stopMusic", stopMusicLua);
    lua_register(L, "toggleAudio", toggleAudioLua);
    lua_register(L, "getVolume", getVolumeLua);
    lua_register(L, "setVolume", setVolumeLua);
    lua_register(L, "stopAllSounds", stopAllSoundsLua);

    //lights
    lua_register(L, "setDarkness", setDarknessLua);
    lua_register(L, "createLight", createLightLua);

    //Text funtions

    lua_register(L, "createTextObject", createTextObjectLua);
    lua_register(L, "setTextObjectColor", setTextObjectColorLua);
    lua_register(L, "setTextObjectText", setTextObjectTextLua);
    lua_register(L, "setTextObjectFont", setTextObjectFontLua);
    lua_register(L, "setTextObjectFontSize", setTextObjectFontSizeLua);
    lua_register(L, "setTextObjectX", setTextObjectXLua);
    lua_register(L, "setTextObjectY", setTextObjectYLua);
    lua_register(L, "setTextObjectVisible", setTextObjectVisibleLua);

    //Entity functions

    //general
    lua_register(L, "createEntity", createEntityLua);
    lua_register(L, "setActive", setActiveLua);
    lua_register(L, "setX", setXLua);
    lua_register(L, "setY", setYLua);
    lua_register(L, "getX", getXLua);
    lua_register(L, "getY", getYLua);
    lua_register(L, "setStatic", setStaticLua);
    lua_register(L, "setVelocityX", setVelocityXLua);
    lua_register(L, "setVelocityY", setVelocityYLua);
    lua_register(L, "getVelocityX", getVelocityXLua);
    lua_register(L, "getVelocityY", getVelocityYLua);

    //texture
    lua_register(L, "setTexture", setTextureLua);
    lua_register(L, "getTexW", getTexWLua);
    lua_register(L, "getTexH", getTexHLua);
    lua_register(L, "setTexW", setTexWLua);
    lua_register(L, "setTexH", setTexHLua);
    lua_register(L, "setVisible", setVisibleLua);

    //physics
    lua_register(L, "setCollisionBox", setCollisionBoxLua);
    lua_register(L, "entityCollides", entityCollidesLua);
    lua_register(L, "setAffectedByPhysics", setAffectedByPhysicsLua);
    lua_register(L, "getColX", getColXLua);
    lua_register(L, "getColY", getColYLua);
    lua_register(L, "getMass", getMassLua);
    lua_register(L, "setMass", setMassLua);

    //animation
    lua_register(L, "setAnimation", setAnimationLua);
    lua_register(L, "setAnimationNum", setAnimationNumLua);
    lua_register(L, "setSpriteNum", setSpriteNumLua);

    return 0;
}

void lua_backend::run_file(std::string file){
    std::string temp = hash_strings(file);
    int status = luaL_loadbuffer(L, temp.c_str(), temp.size(), file.c_str());
    if (status == LUA_ERRSYNTAX || status == LUA_ERRFILE || lua_pcall(L, 0, 0, 0)){
        fprintf(stderr, "cannot compile script\n");
        lua_error(L);
    }
}


int lua_backend::load_file(std::string file){
    std::string temp = hash_strings(file);
    int status = luaL_loadbuffer(L, temp.c_str(), temp.size(), file.c_str());
    if (status == LUA_ERRSYNTAX || status == LUA_ERRFILE){
        fprintf(stderr, "cannot compile script\n");
        lua_error(L);
        return -1;
    }else{
        return 0;
    }
}

void lua_backend::run_script(std::string script) {
    std::string temp = hash_string(script);
    int status = luaL_loadbuffer(L, temp.c_str(), temp.size(), script.c_str());
    if (status == LUA_ERRSYNTAX || status == LUA_ERRFILE || lua_pcall(L, 0, 0, 0)){
        log(ERROR, "Cannot run Script");
    }
}

void lua_backend::set_global(std::string arg){
    lua_setglobal(L, arg.c_str());
}

void lua_backend::run_global(std::string arg) {
    lua_getglobal(L, arg.c_str());
    lua_pcall(L, 0, 0, 0);
}

void lua_backend::close() {
    lua_close(L);
}

bool lua_backend::replace_string(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

//hash plaintext filenames in scripts in the functions 'playSound','playMusic','keyHeld', 'keyReleased' and 'keyPressed'
//this way, we have no hashing/comparing/copying strings in our main loop
std::string lua_backend::hash_strings(std::string path){
    std::ifstream file;
    file.open(path.c_str());
    std::string result;
    if(file.is_open()){
        std::string temp;
        while(!file.eof()){
            getline(file, temp);
            if(!temp.empty()){
                temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
                while(temp.find("playSound(\"") != std::string::npos) {
                    std::string to_find = "playSound(\"";
                    std::string temp_buf;
                    for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                while(temp.find("playMusic(\"") != std::string::npos) {
                    std::string to_find = "playMusic(\"";
                    std::string temp_buf;
                    for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                while(temp.find("keyHeld(\"") != std::string::npos) {
                    std::string to_find = "keyHeld(\"";
                    std::string temp_buf;
                    for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                while(temp.find("keyPressed(\"") != std::string::npos) {
                    std::string to_find = "keyPressed(\"";
                    std::string temp_buf;
                    for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                while(temp.find("keyReleased(\"") != std::string::npos) {
                    std::string to_find = "keyReleased(\"";
                    std::string temp_buf;
                    for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                while(temp.find("setClickKey(\"") != std::string::npos) {
                    std::string to_find = "setClickKey(\"";
                    std::string temp_buf;
                    for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                //A way for creating annotations in lua
                while(temp.find("@Key") != std::string::npos) {
                    std::string to_find = "\"";
                    std::string temp_buf;
                    std::string next_line;
                    getline(file, next_line);
                    for(unsigned long i = next_line.find(to_find) + to_find.size(); next_line.at(i) != '\"'; i++){
                        temp_buf.push_back((next_line.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(next_line, temp_buf_2, string_hash);
                    temp = next_line;
                }
                while(temp.find("@Audio") != std::string::npos) {
                    std::string to_find = "\"";
                    std::string temp_buf;
                    std::string next_line;
                    getline(file, next_line);
                    for(unsigned long i = next_line.find(to_find) + to_find.size(); next_line.at(i) != '\"'; i++){
                        temp_buf.push_back((next_line.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(next_line, temp_buf_2, string_hash);
                    temp = next_line;
                }
                //Remove all logging from gamecode if compiling for release
                #ifdef __RELEASE
                while(temp.find("log(") != std::string::npos) {
                    temp = "";
                }
                #endif
                result += temp + "\n";
            }
        }
        file.close();
        return result;
    }
    else{
        log(ERROR, "File " + path + " not found");
        return nullptr;
    }
}

//hash plaintext filenames in scripts in the functions 'playSound','playMusic','keyHeld', 'keyReleased' and 'keyPressed'
//this way, we have no hashing/comparing/copying strings in our main loop
std::string lua_backend::hash_string(std::string& temp){
    std::string result;
    if(!temp.empty()){
        temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
        while(temp.find("playSound(\"") != std::string::npos) {
            std::string to_find = "playSound(\"";
            std::string temp_buf;
            for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                temp_buf.push_back((temp.at(i)));
            }
            std::hash<std::string> hash_f;
            std::string string_hash = std::to_string(hash_f(temp_buf));
            std::string temp_buf_2 = "\"" + temp_buf + "\"";
            replace_string(temp, temp_buf_2, string_hash);
        }
        while(temp.find("playMusic(\"") != std::string::npos) {
            std::string to_find = "playMusic(\"";
            std::string temp_buf;
            for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                temp_buf.push_back((temp.at(i)));
            }
            std::hash<std::string> hash_f;
            std::string string_hash = std::to_string(hash_f(temp_buf));
            std::string temp_buf_2 = "\"" + temp_buf + "\"";
            replace_string(temp, temp_buf_2, string_hash);
        }
        while(temp.find("keyHeld(\"") != std::string::npos) {
            std::string to_find = "keyHeld(\"";
            std::string temp_buf;
            for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                temp_buf.push_back((temp.at(i)));
            }
            std::hash<std::string> hash_f;
            std::string string_hash = std::to_string(hash_f(temp_buf));
            std::string temp_buf_2 = "\"" + temp_buf + "\"";
            replace_string(temp, temp_buf_2, string_hash);
        }
        while(temp.find("keyPressed(\"") != std::string::npos) {
            std::string to_find = "keyPressed(\"";
            std::string temp_buf;
            for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                temp_buf.push_back((temp.at(i)));
            }
            std::hash<std::string> hash_f;
            std::string string_hash = std::to_string(hash_f(temp_buf));
            std::string temp_buf_2 = "\"" + temp_buf + "\"";
            replace_string(temp, temp_buf_2, string_hash);
        }
        while(temp.find("keyReleased(\"") != std::string::npos) {
            std::string to_find = "keyReleased(\"";
            std::string temp_buf;
            for(unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                temp_buf.push_back((temp.at(i)));
            }
            std::hash<std::string> hash_f;
            std::string string_hash = std::to_string(hash_f(temp_buf));
            std::string temp_buf_2 = "\"" + temp_buf + "\"";
            replace_string(temp, temp_buf_2, string_hash);
        }
        while(temp.find("setClickKey(\"") != std::string::npos) {
            std::string to_find = "setClickKey(\"";
            std::string temp_buf;
            for (unsigned long i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++) {
                temp_buf.push_back((temp.at(i)));
            }
            std::hash<std::string> hash_f;
            std::string string_hash = std::to_string(hash_f(temp_buf));
            std::string temp_buf_2 = "\"" + temp_buf + "\"";
            replace_string(temp, temp_buf_2, string_hash);
        }
        return temp;
    }
    return "Error\n";
}

//External lua API

extern "C" int setDarknessLua(lua_State* L){
    auto arg = static_cast<char>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_DARKNESS, make_data<char>(arg)));
    return 0;
}

extern "C" int createLightLua(lua_State* L){
    auto origin_x = static_cast<int>(lua_tointeger(L, 1));
    auto origin_y = static_cast<int>(lua_tointeger(L, 2));
    auto radius = static_cast<uint16_t>(lua_tointeger(L, 3));
    auto intensity = static_cast<uint16_t>(lua_tointeger(L, 4));
    auto brightness = static_cast<uint16_t>(lua_tointeger(L, 5));
    ST::light tempLight = ST::light(origin_x, origin_y, radius, intensity, brightness);
    gGame_managerLua->get_level_data()->lights.push_back(tempLight);
    return 0;
}

extern "C" int hashStringLua(lua_State* L){
    auto str = static_cast<std::string>(lua_tostring(L, 1));
    std::hash<std::string> hash_f;
    lua_pushinteger(L, (lua_Integer)hash_f(str));
    return 1;
}

extern "C" int setFullscreenLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_FULLSCREEN, make_data<bool>(arg)));
    return 0;
}

extern "C" int getFullscreenStatusLua(lua_State* L){
    lua_pushboolean(L, gGame_managerLua->fullscreen_status);
    return 1;
}


//Text Object Lua bindings

extern "C" int createTextObjectLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto x = static_cast<int>(lua_tointeger(L, 2));
    auto y = static_cast<int>(lua_tointeger(L, 3));
    auto text_string = static_cast<std::string>(lua_tostring(L, 4));
    auto font = static_cast<std::string>(lua_tostring(L, 5));
    auto size = static_cast<int>(lua_tointeger(L, 6));
    SDL_Color temp_color = {255,255,255,255};
    ST::text temp = ST::text(ID, x, y, temp_color, text_string, font, size);
    gGame_managerLua->get_level_data()->text_objects.push_back(temp);
    return 0;
}

extern "C" int setTextObjectColorLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto r = static_cast<uint8_t>(lua_tointeger(L, 2));
    auto g = static_cast<uint8_t>(lua_tointeger(L, 3));
    auto b = static_cast<uint8_t>(lua_tointeger(L, 4));
    auto a = static_cast<uint8_t>(lua_tointeger(L, 5));
    SDL_Color temp = {r, g, b, a};
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_color(temp);
    return 0;
}

extern "C" int setTextObjectTextLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto text = static_cast<std::string>(lua_tostring(L, 2));
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_text_string(text);
    return 0;
}

extern "C" int setTextObjectFontLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto font = static_cast<std::string>(lua_tostring(L, 2));
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_font(font);
    return 0;
}

extern "C" int setTextObjectFontSizeLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto size = static_cast<uint8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_font_size(size);
    return 0;
}

extern "C" int setTextObjectXLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto x = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_x(x);
    return 0;
}

extern "C" int setTextObjectYLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto y = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_y(y);
    return 0;
}

extern "C" int setTextObjectVisibleLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level_data()->text_objects.at(ID).set_visible(arg);
    return 0;
}


//Entity Lua bindings

//GENERAL//

extern "C" int createEntityLua(lua_State* L){
    auto ID = static_cast<unsigned int>(lua_tointeger(L, 1));
    gGame_managerLua->get_level_data()->entities.emplace_back(ST::entity(ID));
    return 0;
}

extern "C" int setXLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto x = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_x(x);
    return 0;
}

extern "C" int setActiveLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_active(arg);
    return 0;
}

extern "C" int setYLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto y = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_y(y);
    return 0;
}

extern "C" int setVelocityXLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<int16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_velocity_x(arg);
    return 0;
}

extern "C" int setVelocityYLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<int16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_velocity_y(arg);
    return 0;
}

extern "C" int getVelocityXLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level_data()->entities.at(id).get_velocity_x());
    return 1;
}

extern "C" int getVelocityYLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level_data()->entities.at(id).get_velocity_y());
    return 1;
}

extern "C" int setStaticLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_static(arg);
    return 0;
}

extern "C" int getXLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level_data()->entities.at(id).get_x());
    return 1;
}

extern "C" int getYLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level_data()->entities.at(id).get_y());
    return 1;
}

//TEXTURE//

extern "C" int getTexWLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level_data()->entities.at(id).get_tex_w());
    return 1;
}

extern "C" int getTexHLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level_data()->entities.at(id).get_tex_h());
    return 1;
}

extern "C" int setTexWLua(lua_State* L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto texW = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_tex_w(texW);
    return 0;
}

extern "C" int setTexHLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto texH = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_tex_h(texH);
    return 0;
}

extern "C" int setVisibleLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_visible(arg);
    return 0;
}

extern "C" int setTextureLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    std::string arg = (std::string)lua_tostring(L, 2);
    gGame_managerLua->get_level_data()->entities.at(id).set_texture(arg);
    return 0;
}

//PHYSICS//

extern "C" int setAffectedByPhysicsLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_affected_by_physics(arg);
    return 0;
}

extern "C" int entityCollidesLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto id2 = static_cast<uint64_t>(lua_tointeger(L, 2));
    ST::level_data* temp = gGame_managerLua->get_level_data();
    if(temp->entities.at(id).is_active()){
        lua_pushboolean(L,temp->entities.at(id).collides(temp->entities.at(id2)));
    }
    else{
        lua_pushboolean(L,false);
    }
    return 1;
}

extern "C" int setCollisionBoxLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto offset_x = static_cast<int16_t>(lua_tointeger(L, 2));
    auto offset_y = static_cast<int16_t>(lua_tointeger(L, 3));
    auto x = static_cast<int16_t>(lua_tointeger(L, 4));
    auto y = static_cast<int16_t>(lua_tointeger(L, 5));
    gGame_managerLua->get_level_data()->entities.at(id).set_collision_box(offset_x, offset_y, x, y);
    return 0;
}

extern "C" int getColXLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level_data()->entities.at(id).get_col_x());
    return 1;
}

extern "C" int getColYLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level_data()->entities.at(id).get_col_y());
    return 1;
}

extern "C" int getMassLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level_data()->entities.at(id).get_mass());
    return 1;
}

extern "C" int setMassLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_mass(arg);
    return 0;
}

extern "C" int pausePhysicsLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(PAUSE_PHYSICS, nullptr));
    return 0;
}

extern "C" int unpausePhysicsLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(UNPAUSE_PHYSICS, nullptr));
    return 0;
}

//ANIMATION//

extern "C" int setAnimationLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_animation(arg);
    return 0;
}

extern "C" int setAnimationNumLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_animation_num(arg);
    return 0;
}

extern "C" int setSpriteNumLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->entities.at(id).set_sprite_num(arg);
    return 0;
}


//OTHER

extern "C" int switchRendererLua(lua_State* L){
    #ifdef __DEBUG
    std::string arg = lua_tostring(L, 1);
    gMessage_busLua->send_msg(make_msg(RENDERER_SWITCH, make_data<std::string>(arg)));
    #endif
    return 0;
}

extern "C" int setGravityLua(lua_State* L){
    auto arg = static_cast<int>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_GRAVITY, make_data<int>(arg)));
    return 0;
}

extern "C" int setLevelFloorLua(lua_State* L){
    auto arg = static_cast<int>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_FLOOR, make_data<int>(arg)));
    return 0;
}

extern "C" int vsyncOnLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(VSYNC_ON, nullptr));
    return 0;
}

extern "C" int vsyncOffLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(VSYNC_OFF, nullptr));
    return 0;
}

extern "C" int getVsyncStateLua(lua_State* L){
    lua_pushboolean(L, gGame_managerLua->vsync_flag);
    return 1;
}


extern "C" int setBrightnessLua(lua_State* L){
    auto arg = static_cast<float>(lua_tonumber(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_WINDOW_BRIGHTNESS, make_data<float>(arg)));
    return 0;
}

extern "C" int centreCameraLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    gGame_managerLua->get_level_data()->Camera.x = gGame_managerLua->get_level_data()->entities.at(id).get_x() - 1920/4;
    while(gGame_managerLua->get_level_data()->Camera.x < gGame_managerLua->get_level_data()->Camera.limitX1 + 1)
        gGame_managerLua->get_level_data()->Camera.x++;
    while(gGame_managerLua->get_level_data()->Camera.x > gGame_managerLua->get_level_data()->Camera.limitX2 - 1)
        gGame_managerLua->get_level_data()->Camera.x--;
    gGame_managerLua->get_level_data()->Camera.y = gGame_managerLua->get_level_data()->entities.at(id).get_y() - 1080;
    while(gGame_managerLua->get_level_data()->Camera.y < gGame_managerLua->get_level_data()->Camera.limitY1 + 1)
        gGame_managerLua->get_level_data()->Camera.y++;
    while(gGame_managerLua->get_level_data()->Camera.y > gGame_managerLua->get_level_data()->Camera.limitY2 - 1)
        gGame_managerLua->get_level_data()->Camera.y--;
    return 0;
}

extern "C" int setLevelsizeLua(lua_State* L){
    auto x = static_cast<int>(lua_tointeger(L, 1));
    auto y = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level_data()->Camera.limitX2 = x;
    gGame_managerLua->get_level_data()->Camera.limitY2 = y;
    return 0;
}

extern "C" int endGameLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(END_GAME, nullptr));
    return 0;
}

extern "C" int startLevelLua(lua_State* L){
    std::string level = (std::string)lua_tostring(L, 1);
    gMessage_busLua->send_msg(make_msg(START_LEVEL, make_data<std::string>(level)));
    return 0;
}

extern "C" int load_levelLua(lua_State* L){
    std::string arg = (std::string)lua_tostring(L, 1);
    gMessage_busLua->send_msg(make_msg(LOAD_LEVEL, make_data<std::string>(arg)));
    return 0;
}

extern "C" int unload_levelLua(lua_State* L){
    std::string level = (std::string)lua_tostring(L, 1);
    gMessage_busLua->send_msg(make_msg(UNLOAD_LEVEL, make_data<std::string>(level)));
    return 0;
}

extern "C" int delayLua(lua_State* L){
    auto ms = static_cast<unsigned int>(lua_tointeger(L, 1));
    SDL_Delay(ms);
    return 0;
}

extern "C" int useLua(lua_State* L){
    std::string arg = (std::string)lua_tostring(L, 1);
    std::string temp = "levels/";
    temp = temp + gGame_managerLua->get_active_level();
    temp = temp + "/" + arg;
    gLua_backendLua->run_file(temp);
    return 0;
}

extern "C" int showMouseCursorLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(SHOW_MOUSE, make_data<bool>(true)));
    return 0;
}

extern "C" int hideMouseCursorLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(SHOW_MOUSE, make_data<bool>(false)));
    return 0;
}

extern "C" int setBackgroundLua(lua_State* L){
    std::string arg = (std::string)lua_tostring(L, 1);
    std::hash<std::string> hash_f;
    gGame_managerLua->get_level_data()->background = hash_f(arg);
    return 0;
}

extern "C" int setOverlayLua(lua_State* L){
    std::string arg = (std::string)lua_tostring(L, 1);
    auto spriteNum = static_cast<int16_t>(lua_tointeger(L, 2));
    std::hash<std::string> hash_f;
    gGame_managerLua->get_level_data()->overlay = hash_f(arg);
    gGame_managerLua->get_level_data()->overlay_spriteNum = spriteNum;
    return 0;
}

//INPUT

extern "C" int getMouseXLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_mouse_x());
    return 1;
}

extern "C" int getMouseYLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_mouse_y());
    return 1;
}

extern "C" int keyHeldLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->key_held(arg));
    return 1;
}

extern "C" int keyPressedLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->key_pressed(arg));
    return 1;
}

extern "C" int keyReleasedLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->key_released(arg));
    return 1;
}


//AUDIO

extern "C" int toggleAudioLua(lua_State*){
    message* msg_temp = make_msg(TOGGLE_AUDIO, nullptr);
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

extern "C" int playMusicLua(lua_State* L){
    auto arg = (size_t)lua_tointeger(L, 1);
    auto volume = (int)lua_tointeger(L, 2);
    auto loops = (int)lua_tointeger(L, 3);
    std::tuple<size_t, int, int> data = std::make_tuple(arg, volume, loops);
    message* msg_temp = make_msg(PLAY_MUSIC, make_data<std::tuple<size_t, int, int>>(data));
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

extern "C" int playSoundLua(lua_State* L){
    auto arg = (size_t)lua_tointeger(L, 1);
    auto volume = (int)lua_tointeger(L, 2);
    auto loops = (int)lua_tointeger(L, 3);
    std::tuple<size_t, int, int> data = std::make_tuple(arg, volume, loops);
    message* msg_temp = make_msg(PLAY_SOUND, make_data<std::tuple<size_t, int, int>>(data));
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

extern "C" int stopMusicLua(lua_State*){
    message* msg_temp = make_msg(STOP_MUSIC, nullptr);
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

extern "C" int stopAllSoundsLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(STOP_ALL_SOUNDS, nullptr));
    return 0;
}

extern "C" int getVolumeLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->volume_level);
    return 1;
}

extern "C" int setVolumeLua(lua_State* L){
    auto arg = (uint8_t)lua_tointeger(L, 1);
    gMessage_busLua->send_msg(make_msg(SET_VOLUME, make_data<uint8_t>(arg)));
    return 0;
}

#ifdef __DEBUG
extern "C" int showCollisionsLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SHOW_COLLISIONS, make_data<bool>(arg)));
    return 0;
}

extern "C" int logLua(lua_State* L){
    auto type = static_cast<uint8_t>(lua_tointeger(L, 1));
    auto arg = static_cast<std::string>(lua_tostring(L, 2));
    gMessage_busLua->send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log((ST::log_type)type, arg))));
    return 0;
}

extern "C" int showFpsLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SHOW_FPS, make_data<bool>(arg)));
    return 0;
}
#endif