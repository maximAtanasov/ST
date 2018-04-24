/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef LUA_BACKEND_HPP
#define LUA_BACKEND_HPP

#include <message_bus/message_bus.hpp>
#include <defs.hpp>

extern "C" {
    #include <lua5.3/lua.h>
    #include <lua5.3/lualib.h>
    #include <lua5.3/lauxlib.h>
}

//Forward declare the game_manager to avoid a circular dependency.
class game_manager;

///This class handles all interaction with lua.
/**
 * It loads lua scripts and runs them.
 * Also provides Lua with the API to the internal engine code.
 * This is the bridge between gameplay and engine code.
 */
class lua_backend {
private:
    lua_State* L;
    message_bus* gMessage_bus;
    std::string hash_strings(std::string& path);
    std::string hash_string(std::string& path);

public:
    int initialize(message_bus* msg_bus, game_manager* game_mngr);
    void set_global(std::string arg);
    void run_global(std::string arg);
    void run_file(std::string arg);
    int load_file(std::string);
    void run_script(std::string script);
    void close();
};

//Lua binding declarations

extern "C" int setFullscreenLua(lua_State* L);
extern "C" int getFullscreenStatusLua(lua_State* L);
extern "C" int setDarknessLua(lua_State* L);
extern "C" int createLightLua(lua_State* L);
extern "C" int hashStringLua(lua_State* L);
extern "C" int setGravityLua(lua_State* L);
extern "C" int setLevelFloorLua(lua_State* L);
extern "C" int setBackgroundLua(lua_State* L);
extern "C" int setOverlayLua(lua_State* L);
extern "C" int delayLua(lua_State* L);
extern "C" int startLevelLua(lua_State* L);
extern "C" int useLua(lua_State* L);
extern "C" int load_levelLua(lua_State* L);
extern "C" int unload_levelLua(lua_State* L);
extern "C" int endGameLua(lua_State* L);
extern "C" int setLevelsizeLua(lua_State* L);
extern "C" int centreCameraLua(lua_State* L);
extern "C" int showMouseCursorLua(lua_State* L);
extern "C" int hideMouseCursorLua(lua_State* L);
extern "C" int setBrightnessLua(lua_State* L);
extern "C" int vsyncOnLua(lua_State* L);
extern "C" int vsyncOffLua(lua_State* L);
extern "C" int getVsyncStateLua(lua_State* L);
extern "C" int getVolumeLua(lua_State* L);
extern "C" int playMusicLua(lua_State* L);
extern "C" int playSoundLua(lua_State* L);
extern "C" int stopMusicLua(lua_State* L);
extern "C" int stopAllSoundsLua(lua_State* L);
extern "C" int toggleAudioLua(lua_State* L);
extern "C" int keyPressedLua(lua_State* L);
extern "C" int keyHeldLua(lua_State* L);
extern "C" int keyReleasedLua(lua_State* L);
extern "C" int getMouseXLua(lua_State* L);
extern "C" int getMouseYLua(lua_State* L);
extern "C" int setVolumeLua(lua_State* L);

//Text Object lua bindings definitions

extern "C" int createTextObjectLua(lua_State* L);
extern "C" int setTextObjectColorLua(lua_State* L);
extern "C" int setTextObjectTextLua(lua_State* L);
extern "C" int setTextObjectFontLua(lua_State* L);
extern "C" int setTextObjectFontSizeLua(lua_State* L);
extern "C" int setTextObjectXLua(lua_State* L);
extern "C" int setTextObjectYLua(lua_State* L);
extern "C" int setTextObjectVisibleLua(lua_State* L);

//Enitity lua bindigs definitions

//general
extern "C" int createEntityLua(lua_State* L);
extern "C" int setActiveLua(lua_State* L);
extern "C" int setXLua(lua_State* L);
extern "C" int setYLua(lua_State* L);
extern "C" int getXLua(lua_State* L);
extern "C" int getYLua(lua_State* L);
extern "C" int setStaticLua(lua_State* L);
extern "C" int setVelocityXLua(lua_State* L);
extern "C" int setVelocityYLua(lua_State* L);
extern "C" int getVelocityXLua(lua_State* L);
extern "C" int getVelocityYLua(lua_State* L);

//texture
extern "C" int setTextureLua(lua_State* L);
extern "C" int setVisibleLua(lua_State* L);
extern "C" int getTexWLua(lua_State* L);
extern "C" int getTexHLua(lua_State* L);
extern "C" int setTexWLua(lua_State* L);
extern "C" int setTexHLua(lua_State* L);

//physics
extern "C" int setCollisionBoxLua(lua_State* L);
extern "C" int entityCollidesLua(lua_State* L);
extern "C" int setAffectedByPhysicsLua(lua_State* L);
extern "C" int getColXLua(lua_State* L);
extern "C" int getColYLua(lua_State* L);
extern "C" int getMassLua(lua_State* L);
extern "C" int setMassLua(lua_State* L);

extern "C" int pausePhysicsLua(lua_State* L);
extern "C" int unpausePhysicsLua(lua_State* L);

//animation
extern "C" int setAnimationLua(lua_State* L);
extern "C" int setSpriteNumLua(lua_State* L);
extern "C" int setAnimationNumLua(lua_State* L);

#ifdef __DEBUG
extern "C" int showCollisionsLua(lua_State* L);
extern "C" int logLua(lua_State* L);
extern "C" int showFpsLua(lua_State* L);
#endif

#endif
