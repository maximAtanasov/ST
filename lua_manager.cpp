#include "lua_manager.hpp"
#include "level_manager.hpp"



lua_manager::lua_manager(){
    //empty constructor
}

//Register all binding functions to lua and start it
int lua_manager::initialize(){
    L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dofile(L, "levels/general");

    //register lua binding functions

    //General Functions
    lua_register(L, "delay", delayLua);
    lua_register(L, "use", useLua);
    lua_register(L, "randomInt", randomIntLua);
    lua_register(L, "vsyncOn", vsyncOnLua);
    lua_register(L, "vsyncOff", vsyncOffLua);
    lua_register(L, "getVsync", getVsyncLua);
    lua_register(L, "setBrightness", setBrightnessLua);
    lua_register(L, "getVolume", getVolumeLua);
    lua_register(L, "startLevelLua", startLevelLua);
    lua_register(L, "showMouseCursor", showMouseCursorLua);
    lua_register(L, "hideMouseCursor", hideMouseCursorLua);
    lua_register(L, "endGame", endGameLua);
    lua_register(L, "centreCamera", centreCameraLua);
    lua_register(L, "setLevelSize", setLevelsizeLua);
    lua_register(L, "setGravity", setGravityLua);
    lua_register(L, "setLevelFloor", setLevelFloorLua);
    lua_register(L, "toggleCollisions", toggleCollisionsLua);
    lua_register(L, "toggleConsole", toggleConsoleLua);
    lua_register(L, "loadLevel", load_levelLua);
    lua_register(L, "unloadLevel", unload_levelLua);

    //Drawing functions
    lua_register(L, "setBackground", setBackgroundLua);
    lua_register(L, "showScreen", showScreenLua);
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

    //Entity functions

        //general
    lua_register(L, "createEntity", createEntityLua);
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

void lua_manager::run_file(std::string file){
    luaL_dofile(L, file.c_str());
}

void lua_manager::set_global(std::string arg){
    lua_setglobal(L, arg.c_str());
}

int lua_manager::load_file(std::string file){
    return luaL_loadfile(L, file.c_str());
}

void lua_manager::close(){
    lua_close(L);
}




//External lua API

extern "C" int randomIntLua(lua_State* L){
    int x = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int y = lua_tointeger(gLua_manager.get_lua_state(), 2);
    lua_pushinteger(L, rand() % y + x);
    return 1;
}

//GENERAL//

extern "C" int createEntityLua(lua_State* L){
    int ID = lua_tointeger(gLua_manager.get_lua_state(), 1);
    entity* tempEntity = new entity(ID);
    gLevel_manager.get_level_data()->entities.push_back(tempEntity);
    return 0;
}

extern "C" int setXLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    float x = lua_tonumber(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setX(x);
    return 0;
}

extern "C" int setYLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    float y = lua_tonumber(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setY(y);
    return 0;
}

extern "C" int setVelocityXLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setVelocityX(arg);
    return 0;
}

extern "C" int setVelocityYLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setVelocityY(arg);
    return 0;
}

extern "C" int getVelocityXLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state() , gLevel_manager.get_level_data()->entities[id]->getVelocityX());
    return 1;
}

extern "C" int getVelocityYLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state() , gLevel_manager.get_level_data()->entities[id]->getVelocityY());
    return 1;
}

extern "C" int setStaticLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    bool arg = lua_toboolean(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setStatic(arg);
    return 0;
}

extern "C" int getXLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state() , gLevel_manager.get_level_data()->entities[id]->getX());
    return 1;
}

extern "C" int getYLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state() , gLevel_manager.get_level_data()->entities[id]->getY());
    return 1;
}

//TEXTURE//

extern "C" int getTexWLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state(), gLevel_manager.get_level_data()->entities[id]->getTexW());
    return 1;
}

extern "C" int getTexHLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state(), gLevel_manager.get_level_data()->entities[id]->getTexH());
    return 1;
}

extern "C" int setTexWLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int texW = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setTexW(texW);
    return 0;
}

extern "C" int setTexHLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int texH = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setTexH(texH);
    return 0;
}

extern "C" int setVisibleLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_toboolean(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setVisible(arg);
    return 0;
}

extern "C" int setTextureLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setTexture(arg);
    return 0;
}

//PHYSICS//

extern "C" int setAffectedByPhysicsLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    bool arg = lua_toboolean(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setAffectedByPhysics(arg);
    return 0;
}

extern "C" int entityCollidesLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int id2 = lua_tointeger(gLua_manager.get_lua_state(), 2);
    if(gLevel_manager.get_level_data()->entities[id]->getColX() > 0 && gLevel_manager.get_level_data()->entities[id]->getColY() > 0
    && gLevel_manager.get_level_data()->entities[id2]->getColX() > 0 && gLevel_manager.get_level_data()->entities[id2]->getColX() > 0)
        lua_pushboolean(gLua_manager.get_lua_state(),gLevel_manager.get_level_data()->entities[id]->collides(*gLevel_manager.get_level_data()->entities[id2]));
    else
        lua_pushboolean(gLua_manager.get_lua_state(),false);
    return 1;
}

extern "C" int setCollisionBoxLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    float offsetX = lua_tonumber(gLua_manager.get_lua_state(), 2);
    float offsetY = lua_tonumber(gLua_manager.get_lua_state(), 3);
    float x = lua_tonumber(gLua_manager.get_lua_state(), 4);
    float y = lua_tonumber(gLua_manager.get_lua_state(), 5);
    gLevel_manager.get_level_data()->entities[id]->setCollisionBox(offsetX, offsetY, x, y);
    return 0;
}

extern "C" int getColXLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state(), gLevel_manager.get_level_data()->entities[id]->getColX());
    return 1;
}

extern "C" int getColYLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state(), gLevel_manager.get_level_data()->entities[id]->getColY());
    return 1;
}

extern "C" int getMassLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    lua_pushnumber(gLua_manager.get_lua_state(), gLevel_manager.get_level_data()->entities[id]->getMass());
    return 1;
}

extern "C" int setMassLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setMass(arg);
    return 0;
}


//ANIMATION//

extern "C" int setAnimationLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setAnimation(arg);
    return 0;
}

extern "C" int setAnimationNumLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setAnimationNum(arg);
    return 0;
}

extern "C" int setSpriteNumLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int arg = lua_tointeger(gLua_manager.get_lua_state(), 2);
    gLevel_manager.get_level_data()->entities[id]->setSpriteNum(arg);
    return 0;
}


//OTHER

extern "C" int setGravityLua(lua_State* L){
    int arg = lua_tonumber(gLua_manager.get_lua_state(), 1);
    gravity = arg;
    return 0;
}

extern "C" int setLevelFloorLua(lua_State* L){
    int arg = lua_tonumber(gLua_manager.get_lua_state(), 1);
    Levelfloor = arg;
    return 0;
}

extern "C" int toggleConsoleLua(lua_State* L){
    if(gConsole.is_open())
        gConsole.close();
    else
        gConsole.open();
    return 0;
}

extern "C" int getVsyncLua(lua_State* L){
    lua_pushnumber(gLua_manager.get_lua_state() ,gRenderer.get_vsync());
    return 1;
}

extern "C" int vsyncOnLua(lua_State* L){
    gRenderer.vsync_on();
    return 0;
}

extern "C" int vsyncOffLua(lua_State* L){
    gRenderer.vsync_off();
    return 0;
}


extern "C" int setBrightnessLua(lua_State* L){
    /*float arg = lua_tonumber(gLua_manager.get_lua_state(), 1);
    //SDL_SetWindowBrightness(window, arg);*/
    return 0;
}

extern "C" int toggleCollisionsLua(lua_State* L){
    if(collisionsShown)
        collisionsShown = false;
    else
        collisionsShown = true;
    return 0;
}

extern "C" int centreCameraLua(lua_State* L){
    int id = lua_tointeger(gLua_manager.get_lua_state(), 1);
    camera.x = gLevel_manager.get_level_data()->entities[id]->getX() - 1920/4;
    while(camera.x < camera.limitX1 + 1)
        camera.x++;
    while(camera.x > camera.limitX2 - 1)
        camera.x--;
    camera.y = gLevel_manager.get_level_data()->entities[id]->getY() - 1080;
    while(camera.y < camera.limitY1 + 1)
        camera.y++;
    while(camera.y > camera.limitY2 - 1)
        camera.y--;
    return 0;
}

extern "C" int setLevelsizeLua(lua_State* L){
    int x = lua_tointeger(gLua_manager.get_lua_state(), 1);
    int y = lua_tointeger(gLua_manager.get_lua_state(), 2);
    camera.limitX2 = x;
    camera.limitY2 = y;
    return 0;
}

extern "C" int endGameLua(lua_State* L){
    endGame = 0;
    return 0;
}

extern "C" int startLevelLua(lua_State* L){
    std::string level = lua_tostring(gLua_manager.get_lua_state(), 1);
    gLevel_manager.start_level(level);
    return 0;
}

extern "C" int unload_levelLua(lua_State* L){
    std::string level = lua_tostring(gLua_manager.get_lua_state(), 1);
    gLevel_manager.unload_level(level);
    return 0;
}

extern "C" int delayLua(lua_State* L){
    int ms = lua_tointeger(gLua_manager.get_lua_state(), 1);
    SDL_Delay(ms);
    return 0;
}

extern "C" int showScreenLua(lua_State* L){
    gRenderer.present();
    return 0;
}

extern "C" int useLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    std::string temp = "levels/";
    temp = temp + gLevel_manager.get_active_level();
    temp = temp + "/" + arg;
    luaL_dofile(gLua_manager.get_lua_state(), temp.c_str());
    return 0;
}

extern "C" int showMouseCursorLua(lua_State* L){
    SDL_ShowCursor(SDL_ENABLE);
    return 0;
}

extern "C" int hideMouseCursorLua(lua_State* L){
    SDL_ShowCursor(SDL_DISABLE);
    return 0;
}

extern "C" int setBackgroundLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    std::hash<std::string> hash_f;
    gLevel_manager.get_level_data()->background = hash_f(arg);
    return 0;
}

extern "C" int load_levelLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    gLevel_manager.load_level(arg);
    return 0;
}

//INPUT

extern "C" int getMouseXLua(lua_State* L){
    lua_pushnumber(gLua_manager.get_lua_state(), gInput_manager.get_mouseX());
    return 1;
}

extern "C" int getMouseYLua(lua_State* L){
    lua_pushnumber(gLua_manager.get_lua_state(), gInput_manager.get_mouseY());
    return 1;
}

extern "C" int keyHeldLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    lua_pushboolean(gLua_manager.get_lua_state(), gInput_manager.key_held(arg));
    return 1;
}

extern "C" int keyPressedLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    lua_pushboolean(gLua_manager.get_lua_state(), gInput_manager.key_pressed(arg));
    return 1;
}

extern "C" int keyReleasedLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    lua_pushboolean(gLua_manager.get_lua_state(), gInput_manager.key_released(arg));
    return 1;
}


//AUDIO
extern "C" int getVolumeLua(lua_State* L){
    lua_pushnumber(gLua_manager.get_lua_state(), gAudio_manager.get_volume());
    return 1;
}

extern "C" int toggleAudioLua(lua_State* L){
    if(gAudio_manager.get_volume()   > 0)
        gAudio_manager.mute();
    else
        gAudio_manager.unmute();
    return 0;
}

extern "C" int playMusicLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    gAudio_manager.play_music(arg);
    return 0;
}

extern "C" int playSoundLua(lua_State* L){
    std::string arg = lua_tostring(gLua_manager.get_lua_state(), 1);
    gAudio_manager.play_sound(arg);
    return 0;
}

extern "C" int stopMusicLua(lua_State* L){
    gAudio_manager.stop_music();
    return 0;
}
