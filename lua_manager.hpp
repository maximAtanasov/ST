#ifndef LUA_DEF
#define LUA_DEF

#include "renderer.hpp"
#include "input_manager.hpp"
#include "audio_manager.hpp"
#include "physics.hpp"


extern int endGame;

extern "C" {
    #include "lua5.3/lua.h"
    #include "lua5.3/lualib.h"
    #include "lua5.3/lauxlib.h"
}

class lua_manager{
    private:
        lua_State* L;
    public:
        lua_manager();
        int initialize();
        void close();
        void run_file(std::string);
        void set_global(std::string);
        int load_file(std::string);
        inline void run_global(std::string arg){
            lua_getglobal(L, arg.c_str());
            lua_pcall(L, 0, 0, 0);
        }
        inline lua_State* get_lua_state(){
            return L;
        }
};

extern lua_manager gLua_manager;

extern "C" int randomIntLua(lua_State* L);
extern "C" int setGravityLua(lua_State* L);
extern "C" int setLevelFloorLua(lua_State* L);
extern "C" int setBackgroundLua(lua_State* L);
extern "C" int delayLua(lua_State* L);
extern "C" int startLevelLua(lua_State* L);
extern "C" int showScreenLua(lua_State* L);
extern "C" int useLua(lua_State* L);
extern "C" int load_levelLua(lua_State* L);
extern "C" int unload_levelLua(lua_State* L);
extern "C" int endGameLua(lua_State* L);
extern "C" int setLevelsizeLua(lua_State* L);
extern "C" int centreCameraLua(lua_State* L);
extern "C" int showMouseCursorLua(lua_State* L);
extern "C" int hideMouseCursorLua(lua_State* L);
extern "C" int setBrightnessLua(lua_State* L);
extern "C" int getVsyncLua(lua_State* L);
extern "C" int vsyncOnLua(lua_State* L);
extern "C" int vsyncOffLua(lua_State* L);
extern "C" int toggleCollisionsLua(lua_State* L);
extern "C" int toggleConsoleLua(lua_State* L);
extern "C" int playMusicLua(lua_State* L);
extern "C" int playSoundLua(lua_State* L);
extern "C" int stopMusicLua(lua_State* L);
extern "C" int toggleAudioLua(lua_State* L);
extern "C" int getVolumeLua(lua_State* L);
extern "C" int keyPressedLua(lua_State* L);
extern "C" int keyHeldLua(lua_State* L);
extern "C" int keyReleasedLua(lua_State* L);
extern "C" int getMouseXLua(lua_State* L);
extern "C" int getMouseYLua(lua_State* L);

//Enitity lua bindigs definitions

//general
extern "C" int createEntityLua(lua_State* L);
extern "C" int setXLua(lua_State* L);
extern "C" int setYLua(lua_State* L);
extern "C" int getXLua(lua_State* L);
extern "C" int getYLua(lua_State* L);
extern "C" int setStaticLua(lua_State* L);
extern "C" int destroyEntityLua(lua_State* L);
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

//animation
extern "C" int setAnimationLua(lua_State* L);
extern "C" int setSpriteNumLua(lua_State* L);
extern "C" int setAnimationNumLua(lua_State* L);

#endif
