/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef LUA_BACKEND_HPP
#define LUA_BACKEND_HPP

#include <message_bus.hpp>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
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
    friend class lua_backend_test;
private:
    lua_State* L;
    message_bus* gMessage_bus;
    std::string hash_file(const std::string& path);
    std::string hash_string(const std::string& string);

public:
    int initialize(message_bus* msg_bus, game_manager* game_mngr);
    void set_global(const std::string& arg);
    void run_global(const std::string& arg);
    int8_t run_file(const std::string& file);
    int8_t load_file(const std::string&);
    int8_t run_script(const std::string& script);
    void close();
};

//Lua binding declarations

extern "C" int setFullscreenLua(lua_State* L);
extern "C" int getFullscreenStatusLua(lua_State* L);
extern "C" int setDarknessLua(lua_State* L);
extern "C" int hashStringLua(lua_State* L);
extern "C" int setGravityLua(lua_State* L);
extern "C" int pausePhysicsLua(lua_State* L);
extern "C" int unpausePhysicsLua(lua_State* L);
extern "C" int setLevelFloorLua(lua_State* L);
extern "C" int setBackgroundLua(lua_State* L);
extern "C" int setOverlayLua(lua_State* L);
extern "C" int delayLua(lua_State* L);
extern "C" int startLevelLua(lua_State* L);
extern "C" int reloadLevelLua(lua_State* L);
extern "C" int useLua(lua_State* L);
extern "C" int load_levelLua(lua_State* L);
extern "C" int unload_levelLua(lua_State* L);
extern "C" int loadAssetLua(lua_State* L);
extern "C" int unloadAssetLua(lua_State* L);
extern "C" int endGameLua(lua_State* L);
extern "C" int setLevelsizeLua(lua_State* L);
extern "C" int centerCameraLua(lua_State* L);
extern "C" int showMouseCursorLua(lua_State* L);
extern "C" int hideMouseCursorLua(lua_State* L);
extern "C" int setBrightnessLua(lua_State* L);
extern "C" int setVsyncLua(lua_State* L);
extern "C" int getVsyncStateLua(lua_State* L);
extern "C" int getSoundsVolumeLua(lua_State* L);
extern "C" int getMusicVolumeLua(lua_State* L);
extern "C" int pauseMusicLua(lua_State* L);
extern "C" int playMusicLua(lua_State* L);
extern "C" int playSoundLua(lua_State* L);
extern "C" int stopMusicLua(lua_State* L);
extern "C" int stopAllSoundsLua(lua_State* L);
extern "C" int setAudioEnabledLua(lua_State* L);
extern "C" int isAudioEnabledLua(lua_State* L);
extern "C" int keyPressedLua(lua_State* L);
extern "C" int keyHeldLua(lua_State* L);
extern "C" int keyReleasedLua(lua_State* L);
extern "C" int getMouseXLua(lua_State* L);
extern "C" int getMouseYLua(lua_State* L);
extern "C" int leftTriggerLua(lua_State* L);
extern "C" int rightTriggerLua(lua_State* L);
extern "C" int rightStickHorizontalLua(lua_State* L);
extern "C" int leftStickHorizontalLua(lua_State* L);
extern "C" int rightStickVerticalLua(lua_State* L);
extern "C" int leftStickVerticalLua(lua_State* L);
extern "C" int controllerRumbleLua(lua_State* L);
extern "C" int setMusicVolumeLua(lua_State* L);
extern "C" int setSoundsVolumeLua(lua_State* L);
extern "C" int setInternalResolutionLua(lua_State* L);
extern "C" int setWindowResolutionLua(lua_State* L);

//Text Object lua bindings definitions

extern "C" int createTextObjectLua(lua_State* L);
extern "C" int setTextObjectColorLua(lua_State* L);
extern "C" int setTextObjectTextLua(lua_State* L);
extern "C" int setTextObjectFontLua(lua_State* L);
extern "C" int setTextObjectFontSizeLua(lua_State* L);
extern "C" int setTextObjectXLua(lua_State* L);
extern "C" int setTextObjectYLua(lua_State* L);
extern "C" int setTextObjectVisibleLua(lua_State* L);

//Light bindings definitions
extern "C" int createLightLua(lua_State* L);
extern "C" int setLightOriginXLua(lua_State* L);
extern "C" int getLightOriginXLua(lua_State* L);
extern "C" int setLightOriginYLua(lua_State* L);
extern "C" int getLightOriginYLua(lua_State* L);
extern "C" int getLightRadiusLua(lua_State* L);
extern "C" int setLightRadiusLua(lua_State* L);
extern "C" int getLightIntensityLua(lua_State* L);
extern "C" int setLightIntensityLua(lua_State* L);
extern "C" int getLightBrightnessLua(lua_State* L);
extern "C" int setLightBrightnessLua(lua_State* L);
extern "C" int enableLightingLua(lua_State* L);
extern "C" int setLightStaticLua(lua_State* L);
extern "C" int isLightStaticLua(lua_State* L);

//Enitity lua bindigs definitions

//general
extern "C" int createEntityLua(lua_State* L);
extern "C" int setEntityActiveLua(lua_State *L);
extern "C" int setEntityXLua(lua_State *L);
extern "C" int setEntityYLua(lua_State *L);
extern "C" int getEntityXLua(lua_State *L);
extern "C" int getEntityYLua(lua_State *L);
extern "C" int setEntityStaticLua(lua_State *L);
extern "C" int setEntityVelocityXLua(lua_State *L);
extern "C" int setEntityVelocityYLua(lua_State *L);
extern "C" int getEntityVelocityXLua(lua_State *L);
extern "C" int getEntityVelocityYLua(lua_State *L);

//texture
extern "C" int setEntityTextureLua(lua_State *L);
extern "C" int setEntityVisibleLua(lua_State *L);
extern "C" int getEntityTexWLua(lua_State *L);
extern "C" int getEntityTexHLua(lua_State *L);
extern "C" int setEntityTexWLua(lua_State *L);
extern "C" int setEntityTexHLua(lua_State *L);
extern "C" int setEntityTextureScaleLua(lua_State* L);

//physics
extern "C" int setEntityCollisionBoxLua(lua_State *L);
extern "C" int entityCollidesLua(lua_State* L);
extern "C" int setEntityAffectedByPhysicsLua(lua_State *L);
extern "C" int getEntityColXLua(lua_State *L);
extern "C" int getEntityColYLua(lua_State *L);
extern "C" int getEntityColXOffsetLua(lua_State *L);
extern "C" int getEntityColYOffsetLua(lua_State *L);
extern "C" int getEntityMassLua(lua_State *L);
extern "C" int setEntityMassLua(lua_State *L);

//animation
extern "C" int setEntityAnimationLua(lua_State *L);
extern "C" int setEntitySpriteNumLua(lua_State *L);
extern "C" int setEntityAnimationNumLua(lua_State *L);

extern "C" int showCollisionsLua(lua_State* L);
extern "C" int logLua(lua_State* L);
extern "C" int showFpsLua(lua_State* L);
extern "C" int consoleClearLua(lua_State*);

#endif
