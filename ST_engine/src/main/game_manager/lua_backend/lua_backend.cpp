/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <game_manager/lua_backend/lua_backend.hpp>

//The following trick is used to replace the game_manager with a mock implementation when testing.
#ifndef TESTING_LUA_BACKEND
#include <game_manager/game_manager.hpp>
#elif defined(TESTING_LUA_BACKEND)
#include <test/game_manager/lua_backend/game_manager_mock.hpp>
#endif

#include <ST_util/string_util.hpp>
#include <game_manager/level/light.hpp>
#include "lua_backend.hpp"
#include <fstream>
#include <sstream>
#include <SDL_timer.h>

//local to the file, as lua bindings cannot be in a class
static message_bus*  gMessage_busLua;
static game_manager* gGame_managerLua;
static lua_backend* gLua_backendLua;

/**
 * Initializes the Lua subsytem.
 * @param msg_bus A pointer to the global msg_bus.
 * @param game_mngr A pointer to the game_manager object.
 * @return Returns 0 on success or exits with exit code 1 on failure.
 */
int lua_backend::initialize(message_bus* msg_bus, game_manager* game_mngr) {

    //Set the external dependencies.
    gLua_backendLua = this;
    gGame_managerLua = game_mngr;
    gMessage_busLua = msg_bus;
    gMessage_bus = msg_bus;

    //Initialize Lua
    L = luaL_newstate();
    if(L == nullptr){
        fprintf(stderr, "ERROR: Could not initialize a Lua context");
        exit(1);
    }
    luaL_openlibs(L);

    //register lua binding functions

    lua_register(L, "logLua", logLua);
    lua_register(L, "showCollisions", showCollisionsLua);
    lua_register(L, "showFps", showFpsLua);
    lua_register(L, "consoleClear", consoleClearLua);

    //General Functions
    lua_register(L, "setFullscreenLua", setFullscreenLua);
    lua_register(L, "getFullscreenStatus", getFullscreenStatusLua);
    lua_register(L, "hashString", hashStringLua);
    lua_register(L, "delay", delayLua);
    lua_register(L, "use", useLua);
    lua_register(L, "setVsyncLua", setVsyncLua);
    lua_register(L, "getVsyncState", getVsyncStateLua);
    lua_register(L, "setBrightness", setBrightnessLua);
    lua_register(L, "startLevelLua", startLevelLua);
    lua_register(L, "reloadLevelLua", reloadLevelLua);
    lua_register(L, "showMouseCursor", showMouseCursorLua);
    lua_register(L, "hideMouseCursor", hideMouseCursorLua);
    lua_register(L, "endGame", endGameLua);
    lua_register(L, "centerCamera", centerCameraLua);
    lua_register(L, "centreCamera", centerCameraLua);
    lua_register(L, "setLevelSize", setLevelsizeLua);
    lua_register(L, "setLevelFloor", setLevelFloorLua);
    lua_register(L, "loadLevel", load_levelLua);
    lua_register(L, "unloadLevel", unload_levelLua);
	lua_register(L, "loadAsset", loadAssetLua);
	lua_register(L, "unloadAsset", unloadAssetLua);
    lua_register(L, "setInternalResolution", setInternalResolutionLua);
    lua_register(L, "setWindowResolution", setWindowResolutionLua);

    //Physics functions.
    lua_register(L, "setGravity", setGravityLua);
    lua_register(L, "pausePhysics", pausePhysicsLua);
    lua_register(L, "unpausePhysics", unpausePhysicsLua);

    //Drawing functions
    lua_register(L, "setBackground", setBackgroundLua);
    lua_register(L, "setBackgroundColor", setBackgroundColorLua);
    lua_register(L, "setOverlay", setOverlayLua);

    //Input functions
    lua_register(L, "getMouseX", getMouseXLua);
    lua_register(L, "getMouseY", getMouseYLua);
    lua_register(L, "keyHeld", keyHeldLua);
    lua_register(L, "keyPressed", keyPressedLua);
    lua_register(L, "keyReleased", keyReleasedLua);
    lua_register(L, "leftTrigger", leftTriggerLua);
    lua_register(L, "rightTrigger", rightTriggerLua);
    lua_register(L, "rightStickHorizontal", rightStickHorizontalLua);
    lua_register(L, "rightStickVertical", rightStickVerticalLua);
    lua_register(L, "leftStickHorizontal", leftStickHorizontalLua);
    lua_register(L, "leftStickVertical", leftStickVerticalLua);
    lua_register(L, "controllerRumble", controllerRumbleLua);

    //Audio functions
    lua_register(L, "playSound", playSoundLua);
    lua_register(L, "playMusic", playMusicLua);
    lua_register(L, "stopMusic", stopMusicLua);
    lua_register(L, "pauseMusic", pauseMusicLua);
    lua_register(L, "setAudioEnabledLua", setAudioEnabledLua);
    lua_register(L, "isAudioEnabled", isAudioEnabledLua);
    lua_register(L, "getSoundsVolume", getSoundsVolumeLua);
    lua_register(L, "getMusicVolume", getMusicVolumeLua);
    lua_register(L, "setSoundsVolume", setSoundsVolumeLua);
    lua_register(L, "setMusicVolume", setMusicVolumeLua);
    lua_register(L, "stopAllSounds", stopAllSoundsLua);

    //lights
    lua_register(L, "setDarkness", setDarknessLua);
    lua_register(L, "enableLighting", enableLightingLua);
    lua_register(L, "createLight", createLightLua);
    lua_register(L, "setLightOriginX", setLightOriginXLua);
    lua_register(L, "getLightOriginX", getLightOriginXLua);
    lua_register(L, "setLightOriginY", setLightOriginYLua);
    lua_register(L, "getLightOriginY", getLightOriginYLua);
    lua_register(L, "setLightIntensity", setLightIntensityLua);
    lua_register(L, "getLightIntensity", getLightIntensityLua);
    lua_register(L, "setLightBrightness", setLightBrightnessLua);
    lua_register(L, "getLightBrightness", getLightBrightnessLua);
    lua_register(L, "setLightRadius", setLightRadiusLua);
    lua_register(L, "getLightRadius", getLightRadiusLua);
    lua_register(L, "setLightStatic", setLightStaticLua);
    lua_register(L, "isLightStatic", isLightStaticLua);

    //Text funtions
    lua_register(L, "createTextObject", createTextObjectLua);
    lua_register(L, "setTextObjectColor", setTextObjectColorLua);
    lua_register(L, "setTextObjectText", setTextObjectTextLua);
    lua_register(L, "setTextObjectFont", setTextObjectFontLua);
    lua_register(L, "setTextObjectX", setTextObjectXLua);
    lua_register(L, "setTextObjectY", setTextObjectYLua);
    lua_register(L, "setTextObjectVisible", setTextObjectVisibleLua);

    //Entity functions

    //general
    lua_register(L, "createEntity", createEntityLua);
    lua_register(L, "setEntityActive", setEntityActiveLua);
    lua_register(L, "setEntityX", setEntityXLua);
    lua_register(L, "setEntityY", setEntityYLua);
    lua_register(L, "getEntityX", getEntityXLua);
    lua_register(L, "getEntityY", getEntityYLua);
    lua_register(L, "setEntityStatic", setEntityStaticLua);
    lua_register(L, "setEntityVelocityX", setEntityVelocityXLua);
    lua_register(L, "setEntityVelocityY", setEntityVelocityYLua);
    lua_register(L, "getEntityVelocityX", getEntityVelocityXLua);
    lua_register(L, "getEntityVelocityY", getEntityVelocityYLua);

    //texture
    lua_register(L, "setEntityTexture", setEntityTextureLua);
    lua_register(L, "getEntityTexW", getEntityTexWLua);
    lua_register(L, "getEntityTexH", getEntityTexHLua);
    lua_register(L, "setEntityTexW", setEntityTexWLua);
    lua_register(L, "setEntityTexH", setEntityTexHLua);
    lua_register(L, "setEntityVisible", setEntityVisibleLua);
    lua_register(L, "setEntityTextureScale", setEntityTextureScaleLua);

    //physics
    lua_register(L, "setEntityCollisionBox", setEntityCollisionBoxLua);
    lua_register(L, "entityCollides", entityCollidesLua);
    lua_register(L, "setEntityAffectedByPhysics", setEntityAffectedByPhysicsLua);
    lua_register(L, "getEntityColX", getEntityColXLua);
    lua_register(L, "getEntityColY", getEntityColYLua);
    lua_register(L, "getEntityColXOffset", getEntityColXOffsetLua);
    lua_register(L, "getEntityColYOffset", getEntityColYOffsetLua);
    lua_register(L, "getEntityMass", getEntityMassLua);
    lua_register(L, "setEntityMass", setEntityMassLua);

    //animation
    lua_register(L, "setEntityAnimation", setEntityAnimationLua);
    lua_register(L, "setEntityAnimationNum", setEntityAnimationNumLua);
    lua_register(L, "setEntitySpriteNum", setEntitySpriteNumLua);

    luaL_dofile(L, "lua/global_properties.lua");
    luaL_dofile(L, "lua/general.lua");
    luaL_dofile(L, "lua/debug.lua");
    luaL_dofile(L, "lua/objects/entity.lua");
    luaL_dofile(L, "lua/objects/text.lua");
    luaL_dofile(L, "lua/objects/light.lua");
    luaL_dofile(L, "lua/ui/button.lua");
    luaL_dofile(L, "lua/ui/label.lua");
    luaL_dofile(L, "lua/ui/checkbox.lua");

    return 0;
}

/**
 * Run a lua script inside the global Lua state.
 * @param file The path to the file.
 * @return -1 on failiure or 0 on success.
 */
int8_t lua_backend::run_file(const std::string& file){
    std::string temp = hash_file(file);
    int status = luaL_loadbuffer(L, temp.c_str(), temp.size(), file.c_str());
    if (status == LUA_ERRSYNTAX || status == LUA_ERRFILE || lua_pcall(L, 0, 0, 0)){
        fprintf(stderr, "cannot compile script\n");
        lua_error(L);
        return -1;
    }else{
        return 0;
    }
}

/**
 * Loads a file into the global Lua State, but does not run it.
 * @param file The path to the file.
 * @return ABORTS THE APP on failure or returns 0 on success.
 */
int8_t lua_backend::load_file(const std::string& file){
    std::string temp = hash_file(file);
    int status = luaL_loadbuffer(L, temp.c_str(), temp.size(), file.c_str());
    if (status == LUA_ERRSYNTAX || status == LUA_ERRFILE){
        fprintf(stderr, "cannot compile script\n");
        lua_error(L);
        return -1;
    }else{
        return 0;
    }
}

/**
 * Run a lua script contained in a string.
 * @param script The Lua Script to run.
 * @return -1 on failure and 0 on success.
 */
int8_t lua_backend::run_script(const std::string& script) {
    std::string temp = hash_string(script);
   // int status = luaL_loadbuffer(L, temp.c_str(), temp.size(), script.c_str());
    if (luaL_dostring(L, temp.c_str())){
        gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("Cannot run script")));
        return -1;
    }else{
        return 0;
    }
}

/**
 * Set a loaded script as a global.
 * @param arg The name of the script to set as global.
 */
void lua_backend::set_global(const std::string& arg){
    lua_setglobal(L, arg.c_str());
}

/**
 * Run a already set global.
 * @param arg The name of the global.
 */
void lua_backend::run_global(const std::string& arg) {
    lua_getglobal(L, arg.c_str());
    lua_pcall(L, 0, 0, 0);
}

/**
 * Close the Lua State.
 */
void lua_backend::close() {
    lua_close(L);
}

/**
 * Hashes plaintext filenames in scripts in the functions 'playSound','playMusic','keyHeld', 'keyReleased' and 'keyPressed'
 * and also the annotations --@Audio and --@Key.This way, we have no hashing/comparing/copying strings in our main loop
 * @param path The path to the file.
 * @return The script with the values hashed.
 */
std::string lua_backend::hash_file(const std::string& path){
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
                    for(uint64_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
                    for(uint64_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
                    for(uint64_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
                    for(uint64_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
                    for(uint64_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
                    for(uint64_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
                        temp_buf.push_back((temp.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(temp, temp_buf_2, string_hash);
                }
                //A way for creating annotations in lua
                while(temp.find("----@Key") != std::string::npos) {
                    std::string to_find = "\"";
                    std::string temp_buf;
                    std::string next_line;
                    getline(file, next_line);
                    for(uint64_t i = next_line.find(to_find) + to_find.size(); next_line.at(i) != '\"'; i++){
                        temp_buf.push_back((next_line.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(next_line, temp_buf_2, string_hash);
                    temp = next_line;
                }
                while(temp.find("----@Audio") != std::string::npos) {
                    std::string to_find = "\"";
                    std::string temp_buf;
                    std::string next_line;
                    getline(file, next_line);
                    for(uint64_t i = next_line.find(to_find) + to_find.size(); next_line.at(i) != '\"'; i++){
                        temp_buf.push_back((next_line.at(i)));
                    }
                    std::hash<std::string> hash_f;
                    std::string string_hash = std::to_string(hash_f(temp_buf));
                    std::string temp_buf_2 = "\"" + temp_buf + "\"";
                    replace_string(next_line, temp_buf_2, string_hash);
                    temp = next_line;
                }
                result += temp + "\n";
            }
        }
        file.close();
        return result;
    }
    else{
        gMessage_bus->send_msg(make_msg(LOG_ERROR, make_data<std::string>("File " + path + " not found")));
        return nullptr;
    }
}

/**
 * Hashes plaintext filenames in scripts in the functions 'playSound','playMusic','keyHeld', 'keyReleased' and 'keyPressed'
 * and also the annotations --@Audio and --@Key.This way, we have no hashing/comparing/copying strings in our main loop
 * @param path The script.
 * @return The script with the values hashed.
 */
std::string lua_backend::hash_string(const std::string& arg){
    std::string result;
    std::string temp = arg;
    if(!temp.empty()){
        temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
        while(temp.find("playSound(\"") != std::string::npos) {
            std::string to_find = "playSound(\"";
            std::string temp_buf;
            for(size_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
            for(size_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
            for(size_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
            for(size_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
            for(size_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++){
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
            for (size_t i = temp.find(to_find) + to_find.size(); temp.at(i) != '\"'; i++) {
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

/**
 * Sends a SET_DARKNESS message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setDarknessLua(lua_State* L){
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_DARKNESS, make_data<uint8_t>(arg)));
    return 0;
}

extern "C" int enableLightingLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(ENABLE_LIGHTING, make_data(arg)));
    return 0;
}

/**
 * Creates a new Light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int createLightLua(lua_State* L){
    auto ID = static_cast<uint64_t >(lua_tointeger(L, 1));
    auto origin_x = static_cast<int32_t>(lua_tointeger(L, 2));
    auto origin_y = static_cast<int32_t>(lua_tointeger(L, 3));
    auto radius = static_cast<uint16_t>(lua_tointeger(L, 4));
    auto intensity = static_cast<uint16_t>(lua_tointeger(L, 5));
    auto brightness = static_cast<uint16_t>(lua_tointeger(L, 6));
    ST::light tempLight = ST::light(origin_x, origin_y, radius, intensity, brightness);
    gGame_managerLua->get_level()->lights.emplace_back(tempLight);
    return 0;
}

/**
 * Sets the x origin of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLightOriginXLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto origin_x = static_cast<int32_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->lights.at(ID).origin_x = origin_x;
    return 0;
}

/**
 * Gets the x origin of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getLightOriginXLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushinteger(L, gGame_managerLua->get_level()->lights.at(ID).origin_x);
    return 1;
}

/**
 * Sets the y origin of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLightOriginYLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto origin_y = static_cast<int32_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->lights.at(ID).origin_y = origin_y;
    return 0;
}

/**
 * Gets the y origin of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getLightOriginYLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushinteger(L, gGame_managerLua->get_level()->lights.at(ID).origin_y);
    return 1;
}

/**
 * Gets the radius of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getLightRadiusLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushinteger(L, gGame_managerLua->get_level()->lights.at(ID).radius);
    return 1;
}

/**
 * Sets the radius of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLightRadiusLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto radius = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->lights.at(ID).radius = radius;
    return 0;
}

/**
 * Gets the intensity of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getLightIntensityLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushinteger(L, gGame_managerLua->get_level()->lights.at(ID).intensity);
    return 1;
}

/**
 * Sets the intensity of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLightIntensityLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto intensity = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->lights.at(ID).intensity = intensity;
    return 0;
}

/**
 * Gets the brightness of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getLightBrightnessLua(lua_State* L) {
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushinteger(L, gGame_managerLua->get_level()->lights.at(ID).brightness);
    return 1;
}

/**
 * Sets the brightness of a light object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLightBrightnessLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto brightness = static_cast<uint16_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->lights.at(ID).brightness = brightness;
    return 0;
}

/**
 * Sets if the light is static or not(moves with camera).
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLightStaticLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto is_static = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level()->lights.at(ID).is_static = is_static;
    return 0;
}

/**
 * Tells if the light is static or not(moves with camera).
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int isLightStaticLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->get_level()->lights.at(ID).is_static);
    return 1;
}

/**
 * Provides Lua with std::hash<std::string>().
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int hashStringLua(lua_State* L){
    auto str = static_cast<std::string>(lua_tostring(L, 1));
    std::hash<std::string> hash_f;
    lua_pushinteger(L, (lua_Integer)hash_f(str));
    return 1;
}

/**
 * Sends a SET_FULLSCREEN message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setFullscreenLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_FULLSCREEN, make_data<bool>(arg)));
    return 0;
}

/**
 * Asks the game_manager if we are running in fullscreen or not.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getFullscreenStatusLua(lua_State* L){
    lua_pushboolean(L, gGame_managerLua->fullscreen_status);
    return 1;
}


//Text Object Lua bindings

/**
 * Creates a new text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int createTextObjectLua(lua_State* L){
    auto x = static_cast<int>(lua_tointeger(L, 1));
    auto y = static_cast<int>(lua_tointeger(L, 2));
    auto text_string = static_cast<std::string>(lua_tostring(L, 3));
    auto font = static_cast<std::string>(lua_tostring(L, 4));
    auto size = static_cast<uint8_t>(lua_tointeger(L, 5));
    SDL_Color temp_color = {255,255,255,255};

    std::hash<std::string> hash_f;
    ST::text temp = ST::text(x, y, temp_color, text_string, hash_f(font + " " + std::to_string(size)));
    gGame_managerLua->get_level()->text_objects.emplace_back(temp);
    return 0;
}

/**
 * Sets the color of the text in a text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setTextObjectColorLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto r = static_cast<uint8_t>(lua_tointeger(L, 2));
    auto g = static_cast<uint8_t>(lua_tointeger(L, 3));
    auto b = static_cast<uint8_t>(lua_tointeger(L, 4));
    auto a = static_cast<uint8_t>(lua_tointeger(L, 5));
    gGame_managerLua->get_level()->text_objects.at(ID).color = {r,g,b,a};
    return 0;
}

/**
 * Sets the text a text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setTextObjectTextLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto text = static_cast<std::string>(lua_tostring(L, 2));
    gGame_managerLua->get_level()->text_objects.at(ID).text_string = text;
    return 0;
}

/**
 * Sets the font of a text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setTextObjectFontLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto font = static_cast<std::string>(lua_tostring(L, 2));
    std::hash<std::string> hash_f;
    gGame_managerLua->get_level()->text_objects.at(ID).font = hash_f(font);
    return 0;
}

/**
 * Sets the x position of a text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setTextObjectXLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto x = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->text_objects.at(ID).x = x;
    return 0;
}

/**
 * Sets the y position of a text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setTextObjectYLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto y = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->text_objects.at(ID).y = y;
    return 0;
}

/**
 * Sets the visibility of a text object.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setTextObjectVisibleLua(lua_State* L){
    auto ID = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level()->text_objects.at(ID).is_visible = arg;
    return 0;
}


//Entity Lua bindings

/**
 * Creates a new entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int createEntityLua(lua_State*){
    gGame_managerLua->get_level()->entities.emplace_back(ST::entity());
    return 0;
}

/**
 * Sets the x position of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityXLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto x = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).x = x;
    return 0;
}

/**
 * Sets if entity an entity is active or not.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityActiveLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level()->entities.at(id).is_active = arg;
    return 0;
}

/**
 * Sets the y position of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityYLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto y = static_cast<int>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).y = y;
    return 0;
}

/**
 * Sets the x velocity of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityVelocityXLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<int8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).velocity_x = arg;
    return 0;
}

/**
 * Sets the y velocity of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityVelocityYLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<int8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).velocity_y = arg;
    return 0;
}

/**
 * Gets the x velocity of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityVelocityXLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level()->entities.at(id).velocity_x);
    return 1;
}

/**
 * Gets the y velocity of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityVelocityYLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level()->entities.at(id).velocity_y);
    return 1;
}

/**
 * Sets the entity to static/non-static.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityStaticLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level()->entities.at(id).is_static = arg;
    return 0;
}

/**
 * Gets the x position of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityXLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level()->entities.at(id).x);
    return 1;
}

/**
 * Gets the y position of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityYLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L , gGame_managerLua->get_level()->entities.at(id).y);
    return 1;
}

//TEXTURE//

/**
 * Gets the texture width of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityTexWLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).tex_w);
    return 1;
}

/**
 * Gets the texture height of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityTexHLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).tex_h);
    return 1;
}

/**
 * Sets the texture width of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityTexWLua(lua_State *L){
    auto id = static_cast<unsigned long>(lua_tonumber(L, 1));
    auto tex_w = static_cast<uint16_t>(lua_tonumber(L, 2));
    gGame_managerLua->get_level()->entities.at(id).tex_w = tex_w;
    return 0;
}

/**
 * Sets the texture height of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityTexHLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tonumber(L, 1));
    auto tex_h = static_cast<uint16_t>(lua_tonumber(L, 2));
    gGame_managerLua->get_level()->entities.at(id).tex_h = tex_h;
    return 0;
}

/**
 * Sets the texture scale (x and y) of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityTextureScaleLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto scale_x = static_cast<float>(lua_tonumber(L, 2));
    auto scale_y = static_cast<float>(lua_tonumber(L, 3));
    gGame_managerLua->get_level()->entities.at(id).tex_scale_x = scale_x;
    gGame_managerLua->get_level()->entities.at(id).tex_scale_y = scale_y;
    return 0;
}

/**
 * Sets the the entity to visible/non-visible.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityVisibleLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level()->entities.at(id).is_visible = arg;
    return 0;
}

/**
 * Sets the texture of the entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityTextureLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    std::string arg = static_cast<std::string>(lua_tostring(L, 2));
    gGame_managerLua->get_level()->entities.at(id).texture = hash_string(arg);
    return 0;
}

//PHYSICS//

/**
 * Sets the physcis status of an entity (affected/not affected).
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityAffectedByPhysicsLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<bool>(lua_toboolean(L, 2));
    gGame_managerLua->get_level()->entities.at(id).is_affected_by_physics = arg;
    return 0;
}

/**
 * Tells if two entities are colliding.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int entityCollidesLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto id2 = static_cast<uint64_t>(lua_tointeger(L, 2));
    ST::level* temp = gGame_managerLua->get_level();
    if(temp->entities.at(id).is_active){
        lua_pushboolean(L,temp->entities.at(id).collides(temp->entities.at(id2)));
    }
    else{
        lua_pushboolean(L,false);
    }
    return 1;
}

/**
 * Sets the collision box of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityCollisionBoxLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto offset_x = static_cast<int16_t>(lua_tonumber(L, 2));
    auto offset_y = static_cast<int16_t>(lua_tonumber(L, 3));
    auto x = static_cast<int16_t>(lua_tonumber(L, 4));
    auto y = static_cast<int16_t>(lua_tonumber(L, 5));
    gGame_managerLua->get_level()->entities.at(id).set_collision_box(offset_x, offset_y, x, y);
    return 0;
}

/**
 * Gets the width of the collision box of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityColXLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).get_col_x());
    return 1;
}

/**
 * Gets the height of the collision box of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityColYLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).get_col_y());
    return 1;
}

/**
 * Gets the width offset of the collision box of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityColXOffsetLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).get_col_x_offset());
    return 1;
}

/**
 * Gets the height offset of the collision box of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityColYOffsetLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).get_col_y_offset());
    return 1;
}

/**
 * Gets the mass of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getEntityMassLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    lua_pushnumber(L, gGame_managerLua->get_level()->entities.at(id).mass);
    return 1;
}

/**
 * Sets the mass of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityMassLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).mass = arg;
    return 0;
}

/**
 * Pauses physics.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int pausePhysicsLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(PAUSE_PHYSICS, nullptr));
    return 0;
}

/**
 * Unpauses physics.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int unpausePhysicsLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(UNPAUSE_PHYSICS, nullptr));
    return 0;
}

//ANIMATION//

/**
 * Sets the current animation of an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityAnimationLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).animation = arg;
    return 0;
}

/**
 * Sets the number of animations of an entity. (colums in the spritesheet)
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntityAnimationNumLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).animation_num = arg;
    return 0;
}

/**
 * Sets the number of sprites of an entity. (rows in the spritesheet)
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setEntitySpriteNumLua(lua_State *L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->entities.at(id).sprite_num = arg;
    return 0;
}


//OTHER

/**
 * Sets the gravity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setGravityLua(lua_State* L){
    auto arg = static_cast<int8_t>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_GRAVITY, make_data<>(arg)));
    return 0;
}

/**
 * Sets the level floor.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setLevelFloorLua(lua_State* L){
    auto arg = static_cast<int32_t>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_FLOOR, make_data<>(arg)));
    return 0;
}

/**
 * Sets VSYNC on or off.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int setVsyncLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_VSYNC, make_data<>(arg)));
    return 0;
}

/**
 * Get the state of vsync.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int getVsyncStateLua(lua_State* L){
    lua_pushboolean(L, gGame_managerLua->vsync_flag);
    return 1;
}

/**
 * Set the internal rendering resolution.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setInternalResolutionLua(lua_State* L){
    auto width = static_cast<int16_t>(lua_tointeger(L, 1));
    auto height = static_cast<int16_t>(lua_tointeger(L, 2));
    gMessage_busLua->send_msg(make_msg(SET_INTERNAL_RESOLUTION, make_data(std::make_tuple(width, height))));
    return 0;
}

/**
 * Set the resolution for the window.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
int setWindowResolutionLua(lua_State *L) {
    auto width = static_cast<int16_t>(lua_tointeger(L, 1));
    auto height = static_cast<int16_t>(lua_tointeger(L, 2));
    gMessage_busLua->send_msg(make_msg(SET_WINDOW_RESOLUTION, make_data(std::make_tuple(width, height))));
    return 0;
}

/**
 * Set the brightness of the screen.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setBrightnessLua(lua_State* L){
    auto arg = static_cast<float>(lua_tonumber(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_WINDOW_BRIGHTNESS, make_data<>(arg)));
    return 0;
}

/**
 * Center the camera on an entity.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int centerCameraLua(lua_State* L){
    auto id = static_cast<uint64_t>(lua_tointeger(L, 1));
    gGame_managerLua->center_camera_on_entity(id);
    return 0;
}

/**
 * Set the size of the level.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 1.
 */
extern "C" int setLevelsizeLua(lua_State* L){
    auto x = static_cast<int32_t>(lua_tointeger(L, 1));
    auto y = static_cast<int32_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->Camera.limitX2 = x;
    gGame_managerLua->get_level()->Camera.limitY2 = y;
    return 0;
}

/**
 * Ends the application.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int endGameLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(END_GAME, nullptr));
    return 0;
}

/**
 * Start a level given it's name. Sends a <b>START_LEVEL</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int startLevelLua(lua_State* L){
    std::string level = static_cast<std::string>(lua_tostring(L, 1));
    gMessage_busLua->send_msg(make_msg(START_LEVEL, make_data<std::string>(level)));
    return 0;
}

/**
 * Reloads a level given it's name. Sends a <b>RELOAD_LEVEL</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int reloadLevelLua(lua_State* L){
    std::string level = static_cast<std::string>(lua_tostring(L, 1));
    gMessage_busLua->send_msg(make_msg(RELOAD_LEVEL, make_data<std::string>(level)));
    return 0;
}

/**
 * Load a level given it's name. Sends a <b>LOAD_LEVEL</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int load_levelLua(lua_State* L){
    std::string arg = static_cast<std::string>(lua_tostring(L, 1));
    gMessage_busLua->send_msg(make_msg(LOAD_LEVEL, make_data<std::string>(arg)));
    return 0;
}

/**
 * Unload a level given it's name. Sends a <b>UNLOAD_LEVEL</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int unload_levelLua(lua_State* L){
    std::string level = static_cast<std::string>(lua_tostring(L, 1));
    gMessage_busLua->send_msg(make_msg(UNLOAD_LEVEL, make_data<std::string>(level)));
    return 0;
}

/**
 * Load an asset given it's path. Sends a <b>LOAD_ASSET</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int loadAssetLua(lua_State* L) {
	std::string path = static_cast<std::string>(lua_tostring(L, 1));
	gMessage_busLua->send_msg(make_msg(LOAD_ASSET, make_data<std::string>(path)));
	return 0;
}

/**
 * Unload an asset given it's path. Sends a <b>UNLOAD_ASSET</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int unloadAssetLua(lua_State* L) {
	std::string path = static_cast<std::string>(lua_tostring(L, 1));
	gMessage_busLua->send_msg(make_msg(UNLOAD_ASSET, make_data<std::string>(path)));
	return 0;
}

/**
 * Pause the execution of the game simulation thread by a given amount.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int delayLua(lua_State* L){
    auto ms = static_cast<unsigned int>(lua_tointeger(L, 1));
    SDL_Delay(ms);
    return 0;
}

/**
 * Run a lua script given it's name. The script must be in the same directory as the current level.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int useLua(lua_State* L){
    std::string arg = static_cast<std::string>(lua_tostring(L, 1));
    std::string temp = "levels/";
    temp = temp + gGame_managerLua->get_active_level();
    temp = temp + "/" + arg;
    gLua_backendLua->run_file(temp);
    return 0;
}

/**
 * Show the mouse cursor.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int showMouseCursorLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(SHOW_MOUSE, make_data<bool>(true)));
    return 0;
}

/**
 * Hide the mouse cursor.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int hideMouseCursorLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(SHOW_MOUSE, make_data<bool>(false)));
    return 0;
}

/**
 * Set the background of a level.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setBackgroundLua(lua_State* L){
    std::string arg = static_cast<std::string>(lua_tostring(L, 1));
    gGame_managerLua->get_level()->background = hash_string(arg);
    return 0;
}

/**
 * Set the background color of a level.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setBackgroundColorLua(lua_State* L){
    auto r = static_cast<uint8_t>(lua_tointeger(L, 1));
    auto g = static_cast<uint8_t>(lua_tointeger(L, 2));
    auto b = static_cast<uint8_t>(lua_tointeger(L, 3));
    auto a = static_cast<uint8_t>(lua_tointeger(L, 4));

    gGame_managerLua->get_level()->background_color= {r, g, b, a};
    return 0;
}

/**
 * Set the overlay of a level.
 * See the Lua docs for more information.
 * @param L The global Lua State.
 * @return Always 0.
 */
extern "C" int setOverlayLua(lua_State* L){
    std::string arg = static_cast<std::string>(lua_tostring(L, 1));
    auto spriteNum = static_cast<uint8_t>(lua_tointeger(L, 2));
    gGame_managerLua->get_level()->overlay = hash_string(arg);
    gGame_managerLua->get_level()->overlay_spriteNum = spriteNum;
    return 0;
}

//INPUT

/**
 * Get the X position of the mouse cursor.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int getMouseXLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_mouse_x());
    return 1;
}

/**
 * Get the Y position of the mouse cursor.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int getMouseYLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_mouse_y());
    return 1;
}

extern "C" int rightTriggerLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_right_trigger());
    return 1;
}

extern "C" int leftTriggerLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_left_trigger());
    return 1;
}

extern "C" int rightStickHorizontalLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_right_stick_horizontal());
    return 1;
}

extern "C" int leftStickHorizontalLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_left_stick_horizontal());
    return 1;
}

extern "C" int rightStickVerticalLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_right_stick_vertical());
    return 1;
}

extern "C" int leftStickVerticalLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->get_left_stick_vertical());
    return 1;
}

extern "C" int controllerRumbleLua(lua_State* L){
    auto strength = static_cast<float>(lua_tonumber(L, 1));
    auto duration = static_cast<uint32_t>(lua_tonumber(L, 2));
    gMessage_busLua->send_msg(make_msg(CONTROLLER_RUMBLE, make_data(std::make_tuple(strength, duration))));
    return 0;
}

/**
 * Tells if a key is held or not.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int keyHeldLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->key_held(arg));
    return 1;
}

/**
 * Tells if a key is pressed or not.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int keyPressedLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->key_pressed(arg));
    return 1;
}

/**
 * Tells if a key is released or not.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int keyReleasedLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    lua_pushboolean(L, gGame_managerLua->key_released(arg));
    return 1;
}


//AUDIO

/**
 * Toggles the audio. Sends a <b>SET_AUDIO_ENABLED</b> message.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int setAudioEnabledLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    message* msg_temp = make_msg(SET_AUDIO_ENABLED, make_data<>(arg));
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}


/**
 * Plays music given it's name. Sends a <b>PLAY_MUSIC</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int playMusicLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    auto volume = static_cast<uint8_t>(lua_tointeger(L, 2));
    auto loops = static_cast<int8_t>(lua_tointeger(L, 3));
    std::tuple<size_t, uint8_t, int8_t> data = std::make_tuple(arg, volume, loops);
    message* msg_temp = make_msg(PLAY_MUSIC, make_data<std::tuple<size_t, uint8_t, int8_t>>(data));
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

/**
 * Plays a sound given it's name. Sends a <b>PLAY_SOUND</b> message.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int playSoundLua(lua_State* L){
    auto arg = static_cast<size_t>(lua_tointeger(L, 1));
    auto volume = static_cast<uint8_t>(lua_tointeger(L, 2));
    auto loops = static_cast<int8_t>(lua_tointeger(L, 3));
    std::tuple<size_t, uint8_t, int8_t> data = std::make_tuple(arg, volume, loops);
    message* msg_temp = make_msg(PLAY_SOUND, make_data<std::tuple<size_t, uint8_t, int8_t>>(data));
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

/**
 * Stops the music. Sends a <b>STOP_MUSIC</b> message.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int stopMusicLua(lua_State*){
    message* msg_temp = make_msg(STOP_MUSIC, nullptr);
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

/**
 * Stops all currently playing sounds. Sends a <b>STOP_ALL_SOUNDS</b> message.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int stopAllSoundsLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(STOP_ALL_SOUNDS, nullptr));
    return 0;
}

extern "C" int isAudioEnabledLua(lua_State* L){
    lua_pushboolean(L, gGame_managerLua->audio_enabled);
    return 1;
}


/**
 * Sets the current music volume.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int setMusicVolumeLua(lua_State* L){
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_MUSIC_VOLUME, make_data<uint8_t>(arg)));
    return 0;
}

/**
 * Sets the current music volume.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int setSoundsVolumeLua(lua_State* L){
    auto arg = static_cast<uint8_t>(lua_tointeger(L, 1));
    gMessage_busLua->send_msg(make_msg(SET_SOUNDS_VOLUME, make_data<uint8_t>(arg)));
    return 0;
}

/**
 * Gets the current sounds volume.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int getSoundsVolumeLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->sounds_volume_level);
    return 1;
}

/**
 * Gets the current music volume.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 1.
 */
extern "C" int getMusicVolumeLua(lua_State* L){
    lua_pushnumber(L, gGame_managerLua->music_volume_level);
    return 1;
}

/**
 * Pauses the music. Sends a <b>PAUSE_MUSIC</b> message.
 * See the Lua docs for more information.
 * @return Always 0.
 */
extern "C" int pauseMusicLua(lua_State*){
    message* msg_temp = make_msg(PAUSE_MUSIC, nullptr);
    gMessage_busLua->send_msg(msg_temp);
    return 0;
}

/**
 * Show or hide the collisions and coordinates from rendering.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int showCollisionsLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SHOW_COLLISIONS, make_data<bool>(arg)));
    return 0;
}

/**
 * Logging function for lua - works the same as the log() macro in the engine code.
 * Allows logging to the console from gameplay code.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int logLua(lua_State* L) {
    auto type = static_cast<uint8_t>(lua_tointeger(L, 1));
    auto arg = static_cast<std::string>(lua_tostring(L, 2));
    if (type == 1) {
        gMessage_busLua->send_msg(make_msg(LOG_ERROR, make_data<std::string>(arg)));
    }else if(type == 2){
        gMessage_busLua->send_msg(make_msg(LOG_SUCCESS, make_data<std::string>(arg)));
    }else if(type == 4){
        gMessage_busLua->send_msg(make_msg(LOG_INFO, make_data<std::string>(arg)));
    }
    return 0;
}

/**
 * Show or hide the fps counter from rendering.
 * See the Lua docs for more information.
 * @param L The global Lua state.
 * @return Always 0.
 */
extern "C" int showFpsLua(lua_State* L){
    auto arg = static_cast<bool>(lua_toboolean(L, 1));
    gMessage_busLua->send_msg(make_msg(SHOW_FPS, make_data<bool>(arg)));
    return 0;
}

/**
 * Clears the dev console.
 * @return awlays 0.
 */
extern "C" int consoleClearLua(lua_State*){
    gMessage_busLua->send_msg(make_msg(CONSOLE_CLEAR, nullptr));
    return 0;
}