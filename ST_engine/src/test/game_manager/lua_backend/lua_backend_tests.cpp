/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#define TESTING_LUA_BACKEND

#include <gtest/gtest.h>
#include <game_manager/lua_backend/lua_backend.cpp>
#include <main/timer.hpp>

/// Tests fixture for the lua_backend
class lua_backend_test : public ::testing::Test {

protected:
    lua_backend test_subject{};

    std::string hash_file(const std::string& path){
        return test_subject.hash_file(path);
    }

    std::string hash_string_lua(const std::string& string){
        return test_subject.hash_string(string);
    }

    lua_State* get_lua_state(){
        return test_subject.L;
    }

    message_bus* msg_bus{};
    game_manager* game_mngr{};

    void SetUp() override{
        msg_bus = new message_bus();
        game_mngr = new game_manager(msg_bus, nullptr);
        test_subject.initialize(msg_bus, game_mngr);
    }

    void TearDown() override{
        test_subject.close();
        delete game_mngr;
        delete msg_bus;
    }
};

TEST_F(lua_backend_test, test_hash_file_playSound){
    std::string result = hash_file("lua_scripts/test_script_playSound.lua");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-10);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_playMusic){
    std::string result = hash_file("lua_scripts/test_script_playMusic.lua");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-10);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyHeld){
    std::string result = hash_file("lua_scripts/test_script_keyHeld.lua");
    std::string resulting_integer = result.substr(8, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyPressed){
    std::string result = hash_file("lua_scripts/test_script_keyPressed.lua");
    std::string resulting_integer = result.substr(11, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-2);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyReleased){
    std::string result = hash_file("lua_scripts/test_script_keyReleased.lua");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-2);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_setClickKey){
    std::string result = hash_file("lua_scripts/test_script_setClickKey.lua");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyAnnotation){
    std::string result = hash_file("lua_scripts/test_script_keyAnnotation.lua");
    std::string resulting_integer = result.substr(10, std::string::npos);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_playSound){
    std::string result = hash_string_lua("playSound(\"sound.wav\", 100, 1)");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-9);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_audioAnnotation){
    std::string result = hash_file("lua_scripts/test_script_audioAnnotation.lua");
    std::string resulting_integer = result.substr(12, std::string::npos);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_playMusic){
    std::string result = hash_string_lua("playMusic(\"music.mp3\", 100, 1)");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-9);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_keyHeld){
    std::string result = hash_string_lua("keyHeld(\"JUMP\")");
    std::string resulting_integer = result.substr(8, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-1);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_keyPressed){
    std::string result = hash_string_lua("keyPressed(\"JUMP\")");
    std::string resulting_integer = result.substr(11, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-1);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_keyReleased){
    std::string result = hash_string_lua("keyReleased(\"JUMP\")");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-1);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_setClickKey){
    std::string result = hash_string_lua("setClickKey(\"MOUSE1\")");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-1);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        (void)e;
        FAIL();
    }catch (const std::out_of_range& e){
        (void)e;
        FAIL();
    }
}

TEST_F(lua_backend_test, test_run_simple_script){
    ::testing::internal::CaptureStdout();
    ASSERT_EQ(0, test_subject.run_file("lua_scripts/test_script_simple.lua"));
    ASSERT_EQ("4\n", testing::internal::GetCapturedStdout());
}

TEST_F(lua_backend_test, test_fail_on_broken_script){
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
#ifdef _MSC_VER
    //Exit code 3 on WINDOWS
    ASSERT_EXIT(test_subject.run_file("lua_scripts/test_script_broken.lua"), ::testing::ExitedWithCode(3), ".*");
#else
    //SIGABRT on UNIX
    ASSERT_EXIT(test_subject.run_file("lua_scripts/test_script_broken.lua"), ::testing::KilledBySignal(SIGABRT), ".*");
#endif
}

TEST_F(lua_backend_test, test_call_function_setFullscreenLua){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_FULLSCREEN, &subscriber1);

    //Test
    test_subject.run_script("setFullscreenLua(true)");


    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_FULLSCREEN, result->msg_name);
    ASSERT_TRUE(static_cast<bool>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_hashString){
    //Set up
    std::string test_string = "TEST_STRING";
    size_t expected = ST::hash_string(test_string);

    //Test
    test_subject.run_script("return hashString(\"" + test_string +"\")");
    ASSERT_EQ(expected, lua_tointeger(get_lua_state(), -1));
}

TEST_F(lua_backend_test, test_call_function_delay){
    //Set up
    timer test_timer;
    double runtime = 500;

    //Test
    double start_time = test_timer.time_since_start();
    test_subject.run_script("delay(" + std::to_string(runtime) + ")");
    double end_time = test_timer.time_since_start();

    ASSERT_NEAR(runtime, static_cast<uint32_t>(end_time-start_time), 250);
}

TEST_F(lua_backend_test, test_call_function_setVsyncLua){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_VSYNC, &subscriber1);

    //Test
    test_subject.run_script("setVsyncLua(true)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_VSYNC, result->msg_name);
    ASSERT_TRUE(static_cast<bool>(result->base_data));
}


TEST_F(lua_backend_test, test_call_function_startLevelLua){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(START_LEVEL, &subscriber1);
    std::string level_name = "some_level";

    //Test
    test_subject.run_script("startLevelLua(\"" + level_name + "\")");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(START_LEVEL, result->msg_name);
    ASSERT_EQ(level_name, *static_cast<std::string*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_loadLevel){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(LOAD_LEVEL, &subscriber1);
    std::string level_name = "some_level";

    //Test
    test_subject.run_script("loadLevel(\"" + level_name + "\")");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(LOAD_LEVEL, result->msg_name);
    ASSERT_EQ(level_name, *static_cast<std::string*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_showMouseCursor){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SHOW_MOUSE, &subscriber1);

    //Test
    test_subject.run_script("showMouseCursor()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SHOW_MOUSE, result->msg_name);
    ASSERT_TRUE(static_cast<bool>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_hideMouseCursor){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SHOW_MOUSE, &subscriber1);

    //Test
    test_subject.run_script("hideMouseCursor()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SHOW_MOUSE, result->msg_name);
    ASSERT_FALSE(static_cast<bool>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_endGame){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(END_GAME, &subscriber1);

    //Test
    test_subject.run_script("endGame()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(END_GAME, result->msg_name);
    ASSERT_FALSE(result->get_data());
}

TEST_F(lua_backend_test, test_call_function_setLevelFloor){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_FLOOR, &subscriber1);

    //Test
    test_subject.run_script("setLevelFloor(500)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_FLOOR, result->msg_name);
    ASSERT_EQ(500, static_cast<int32_t>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_unloadLevel){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(UNLOAD_LEVEL, &subscriber1);
    std::string level_name = "some_level";

    //Test
    test_subject.run_script("unloadLevel(\"" + level_name + "\")");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(UNLOAD_LEVEL, result->msg_name);
    ASSERT_EQ(level_name, *static_cast<std::string*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_setGravity){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_GRAVITY, &subscriber1);

    //Test
    test_subject.run_script("setGravity(12)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_GRAVITY, result->msg_name);
    ASSERT_EQ(12, static_cast<uint8_t>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_getGravity){
    //Set up
    gGame_managerLua->gravity = 11;

    //Test
    test_subject.run_script("return getGravity()");

    gGame_managerLua->gravity = 0;

    //Check result
    ASSERT_EQ(11, lua_tointeger(get_lua_state(), -1));
}


TEST_F(lua_backend_test, test_call_function_pausePhysics){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(PAUSE_PHYSICS, &subscriber1);

    //Test
    test_subject.run_script("pausePhysics()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(PAUSE_PHYSICS, result->msg_name);
    ASSERT_FALSE(result->get_data());
}

TEST_F(lua_backend_test, test_call_function_unpausePhysics){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(UNPAUSE_PHYSICS, &subscriber1);

    //Test
    test_subject.run_script("unpausePhysics()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(UNPAUSE_PHYSICS, result->msg_name);
    ASSERT_FALSE(result->get_data());
}

TEST_F(lua_backend_test, test_call_function_playSound){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(PLAY_SOUND, &subscriber1);
    std::string test_string = "some_sound.wav";
    uint16_t expected = ST::hash_string(test_string);

    //Test
    test_subject.run_script("playSound(\"" + test_string + "\", 100, 3)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();
    ASSERT_TRUE(result);
    ASSERT_EQ(PLAY_SOUND, result->msg_name);


    auto data = result->base_data;

    ASSERT_EQ(expected, (data & 0x0000ffff));
    ASSERT_EQ(100, (data >> 16) & 0x000000ff);
    ASSERT_EQ(3, (data >> 24) & 0x000000ff);
}

TEST_F(lua_backend_test, test_call_function_playMusic){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(PLAY_MUSIC, &subscriber1);
    std::string test_string = "some_music.mp3";
    uint16_t expected = ST::hash_string(test_string);

    //Test
    test_subject.run_script("playMusic(\"" + test_string + "\", 128, 2)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();
    ASSERT_TRUE(result);
    ASSERT_EQ(PLAY_MUSIC, result->msg_name);

    auto data = result->base_data;

    ASSERT_EQ(expected, (data & 0x0000ffff));
    ASSERT_EQ(128, (data >> 16) & 0x000000ff);
    ASSERT_EQ(2, (data >> 24) & 0x000000ff);
}

TEST_F(lua_backend_test, test_call_function_stopMusic){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(STOP_MUSIC, &subscriber1);

    //Test
    test_subject.run_script("stopMusic()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(STOP_MUSIC, result->msg_name);
    ASSERT_FALSE(result->get_data());
}

TEST_F(lua_backend_test, test_call_function_setAudioEnabledLua){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_AUDIO_ENABLED, &subscriber1);

    //Test
    test_subject.run_script("setAudioEnabledLua(true)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_AUDIO_ENABLED, result->msg_name);
    ASSERT_TRUE(static_cast<bool>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_stopAllSounds){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(STOP_ALL_SOUNDS, &subscriber1);

    //Test
    test_subject.run_script("stopAllSounds()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(STOP_ALL_SOUNDS, result->msg_name);
    ASSERT_FALSE(result->get_data());
}

TEST_F(lua_backend_test, test_call_function_setMusicVolume){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_MUSIC_VOLUME, &subscriber1);

    //Test
    test_subject.run_script("setMusicVolume(50)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_MUSIC_VOLUME, result->msg_name);
    ASSERT_EQ(50, static_cast<uint8_t>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_setSoundsVolume){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_SOUNDS_VOLUME, &subscriber1);

    //Test
    test_subject.run_script("setSoundsVolume(50)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_SOUNDS_VOLUME, result->msg_name);
    ASSERT_EQ(50, static_cast<uint8_t>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_loadAsset) {
	//Set up
	subscriber subscriber1;
	msg_bus->subscribe(LOAD_ASSET, &subscriber1);

	//Test
	test_subject.run_script("loadAsset(\"path/to/asset.webp\")");

	//Check result - expect to see a message with appropriate content
	message* result = subscriber1.get_next_message();

	ASSERT_TRUE(result);
	ASSERT_EQ(LOAD_ASSET, result->msg_name);
	ASSERT_EQ("path/to/asset.webp", *static_cast<std::string*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_unloadAsset) {
	//Set up
	subscriber subscriber1;
	msg_bus->subscribe(UNLOAD_ASSET, &subscriber1);

	//Test
	test_subject.run_script("unloadAsset(\"path/to/asset.webp\")");

	//Check result - expect to see a message with appropriate content
	message* result = subscriber1.get_next_message();

	ASSERT_TRUE(result);
	ASSERT_EQ(UNLOAD_ASSET, result->msg_name);
	ASSERT_EQ("path/to/asset.webp", *static_cast<std::string*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_setDarkness){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_DARKNESS, &subscriber1);

    //Test
    test_subject.run_script("setDarkness(250)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_DARKNESS, result->msg_name);
    ASSERT_EQ(250, static_cast<uint8_t>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_enableLighting){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(ENABLE_LIGHTING, &subscriber1);

    //Test
    test_subject.run_script("enableLighting(false)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(ENABLE_LIGHTING, result->msg_name);
    ASSERT_FALSE(static_cast<bool>(result->base_data));
}

TEST_F(lua_backend_test, test_call_function_getFullscreenStatus){
    game_mngr->fullscreen_status = false;
    test_subject.run_script("return getFullscreenStatus()");

    //Check result
    ASSERT_FALSE(lua_toboolean(get_lua_state(), -1));
}

TEST_F(lua_backend_test, test_call_function_use){
    //Set up
    ::testing::internal::CaptureStdout();

    //Test
    test_subject.run_script("use(\"test_script_simple.lua\")");
    ASSERT_EQ("4\n", testing::internal::GetCapturedStdout());
}

TEST_F(lua_backend_test, test_call_function_setBackground){
    test_subject.run_script("setBackground(\"some_bg.webp\")");
    ASSERT_EQ(1, game_mngr->get_level_calls);
}

TEST_F(lua_backend_test, test_call_function_setBackgroundColor){
    test_subject.run_script("setBackgroundColor(255, 255, 255, 255)");
    ASSERT_EQ(1, game_mngr->get_level_calls);
}

TEST_F(lua_backend_test, test_call_function_getVsyncState){
    game_mngr->vsync_flag = true;
    test_subject.run_script("return getVsyncState()");

    //Check result
    ASSERT_TRUE(lua_toboolean(get_lua_state(), -1));
}


TEST_F(lua_backend_test, test_call_function_setBrightness){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_WINDOW_BRIGHTNESS, &subscriber1);

    //Test
    test_subject.run_script("setBrightness(250)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_WINDOW_BRIGHTNESS, result->msg_name);
    ASSERT_EQ(250, *static_cast<float*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_setOverlay){
    test_subject.run_script("setOverlay(\"some_bg.webp\", 3)");
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(3, game_mngr->get_level()->overlay_spriteNum);
    ASSERT_EQ(ST::hash_string("some_bg.webp"), game_mngr->get_level()->overlay);
}

TEST_F(lua_backend_test, test_call_function_getMusicVolume){
    game_mngr->music_volume_level = 123;
    test_subject.run_script("return getMusicVolume()");

    //Check result
    ASSERT_EQ(123, lua_tointeger(get_lua_state(), -1));
}

TEST_F(lua_backend_test, test_call_function_getSoundsVolume){
    game_mngr->sounds_volume_level = 123;
    test_subject.run_script("return getSoundsVolume()");

    //Check result
    ASSERT_EQ(123, lua_tointeger(get_lua_state(), -1));
}

TEST_F(lua_backend_test, test_call_function_isAudioEnabled){
    test_subject.run_script("return isAudioEnabled()");

    //Check result
    ASSERT_TRUE(lua_toboolean(get_lua_state(), -1));
}

TEST_F(lua_backend_test, test_call_function_centerCamera){
    test_subject.run_script("centerCamera(5)");  //Test US spelling
    test_subject.run_script("centreCamera(2)");  //Test UK spelling

    //Check result
    ASSERT_EQ(2, game_mngr->center_camera_on_entity_calls);
}

TEST_F(lua_backend_test, test_call_function_setLevelSize){
    test_subject.run_script("setLevelSize(5000, 5000)");

    //Check result
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(5000, game_mngr->get_level()->Camera.limitX2);
    ASSERT_EQ(5000, game_mngr->get_level()->Camera.limitY2);
}

TEST_F(lua_backend_test, test_call_function_getMouseX){
    test_subject.run_script("return getMouseX()");

    //Check result
    ASSERT_EQ(100, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}


TEST_F(lua_backend_test, test_call_function_getMouseY){
    test_subject.run_script("return getMouseY()");

    //Check result
    ASSERT_EQ(200, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_leftTrigger){
    test_subject.run_script("return leftTrigger()");

    //Check result
    ASSERT_EQ(100, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_rightTrigger){
    test_subject.run_script("return rightTrigger()");

    //Check result
    ASSERT_EQ(200, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_rightStickHorizontal){
    test_subject.run_script("return rightStickHorizontal()");

    //Check result
    ASSERT_EQ(400, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_leftStickHorizontal){
    test_subject.run_script("return leftStickHorizontal()");

    //Check result
    ASSERT_EQ(100, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_leftStickVertical){
    test_subject.run_script("return leftStickVertical()");

    //Check result
    ASSERT_EQ(200, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_rightStickVertical){
    test_subject.run_script("return rightStickVertical()");

    //Check result
    ASSERT_EQ(300, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_keyHeld){
    test_subject.run_script("return keyHeld(\"SOME_KEY\")");

    //Check result
    ASSERT_TRUE(lua_toboolean(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_keyPressed){
    test_subject.run_script("return keyPressed(\"SOME_KEY\")");

    //Check result
    ASSERT_TRUE(lua_toboolean(get_lua_state(), lua_gettop(get_lua_state())));
}

TEST_F(lua_backend_test, test_call_function_keyReleased){
    test_subject.run_script("return keyReleased(\"SOME_KEY\")");

    //Check result
    ASSERT_TRUE(lua_toboolean(get_lua_state(), lua_gettop(get_lua_state())));
}


TEST_F(lua_backend_test, test_call_function_createLight){
    test_subject.run_script("createLight(0, 500, 600, 100, 50, 400)");

    //Check results
    ASSERT_EQ(1, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_setLightOriginX){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("setLightOriginX(0, 1000)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(1000, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_setLightOriginY){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("setLightOriginY(0, 1000)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(1000, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_getLightOriginX){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return getLightOriginX(0)");

    //Check results
    ASSERT_EQ(500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Make sure nothing has changed in the light object
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_getLightOriginY){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return getLightOriginY(0)");

    //Check results
    ASSERT_EQ(600, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Make sure nothing has changed in the light object
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_setLightIntensity){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("setLightIntensity(0, 1000)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(1000, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_getLightIntensity){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return getLightIntensity(0)");

    //Check results
    ASSERT_EQ(50, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Make sure nothing has changed in the light object
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_getLightRadius){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return getLightRadius(0)");

    //Check results
    ASSERT_EQ(100, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Make sure nothing has changed in the light object
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_setLightRadius){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return setLightRadius(0, 300)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(300, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_getLightBrightness){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return getLightBrightness(0)");

    //Check results
    ASSERT_EQ(400, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Make sure nothing has changed in the light object
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_setLightBrightness){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return setLightBrightness(0, 300)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(300, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_isLightStatic){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return isLightStatic(0)");

    //Check results
    ASSERT_FALSE(lua_toboolean(get_lua_state(), lua_gettop(get_lua_state())));

    //Make sure nothing has changed in the light object
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
}

TEST_F(lua_backend_test, test_call_function_setLightStatic){
    //Set up
    game_mngr->get_level()->lights.emplace_back(500, 600, 100, 50, 400);

    //Test
    test_subject.run_script("return setLightStatic(0, true)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->lights.size());
    ASSERT_EQ(500, game_mngr->get_level()->lights.at(0).origin_x);
    ASSERT_EQ(600, game_mngr->get_level()->lights.at(0).origin_y);
    ASSERT_EQ(100, game_mngr->get_level()->lights.at(0).radius);
    ASSERT_EQ(50, game_mngr->get_level()->lights.at(0).intensity);
    ASSERT_EQ(400, game_mngr->get_level()->lights.at(0).brightness);
    ASSERT_TRUE(game_mngr->get_level()->lights.at(0).is_static);
}

TEST_F(lua_backend_test, test_call_function_createTextObject){
    test_subject.run_script(R"(createTextObject(500, 600, "SOME_TEXT", "SOME_FONT.ttf", 40))");
    //Check results
    ASSERT_EQ(1, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(500, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(600, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("SOME_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("SOME_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_TRUE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.a);
}


TEST_F(lua_backend_test, test_call_function_setTextObjectColor){
    //Set up
    game_mngr->get_level()->text_objects.emplace_back(ST::text(500, 600, {255,255,255,255},
            "SOME_TEXT", ST::hash_string("SOME_FONT.ttf 40")));

    //Test
    test_subject.run_script("setTextObjectColor(0, 100, 110, 120, 130)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(500, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(600, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("SOME_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("SOME_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_TRUE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(100, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(110, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(120, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(130, game_mngr->get_level()->text_objects.at(0).color.a);
}

TEST_F(lua_backend_test, test_call_function_setTextObjectText){
    //Set up
    game_mngr->get_level()->text_objects.emplace_back(ST::text(500, 600, {255,255,255,255},
                                                                    "SOME_TEXT", ST::hash_string("SOME_FONT.ttf 40")));
    //Test
    test_subject.run_script("setTextObjectText(0, \"NEW_TEXT\")");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(500, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(600, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("NEW_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("SOME_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_TRUE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.a);
}

TEST_F(lua_backend_test, test_call_function_setTextObjectFont){
    //Set up
    game_mngr->get_level()->text_objects.emplace_back(ST::text(500, 600, {255,255,255,255},
                                                                    "SOME_TEXT", ST::hash_string("SOME_FONT.ttf 40")));
    //Test
    test_subject.run_script("setTextObjectFont(0, \"NEW_FONT.ttf 40\")");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(500, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(600, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("SOME_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("NEW_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_TRUE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.a);
}

TEST_F(lua_backend_test, test_call_function_setTextObjectX){
    //Set up
    game_mngr->get_level()->text_objects.emplace_back(
            ST::text(500, 600, {255,255,255,255}, "SOME_TEXT", ST::hash_string("SOME_FONT.ttf 40")));
    //Test
    test_subject.run_script("setTextObjectX(0, 123)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(123, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(600, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("SOME_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("SOME_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_TRUE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.a);
}

TEST_F(lua_backend_test, test_call_function_setTextObjectY){
    //Set up
    game_mngr->get_level()->text_objects.emplace_back(ST::text(500, 600, {255,255,255,255},
                                                                    "SOME_TEXT", ST::hash_string("SOME_FONT.ttf 40")));
    //Test
    test_subject.run_script("setTextObjectY(0, 123)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(500, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(123, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("SOME_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("SOME_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_TRUE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.a);
}

TEST_F(lua_backend_test, test_call_function_setTextObjectVisible){
    //Set up
    game_mngr->get_level()->text_objects.emplace_back(ST::text(500, 600, {255,255,255,255},
                                                                    "SOME_TEXT", ST::hash_string("SOME_FONT.ttf 40")));
    //Test
    test_subject.run_script("setTextObjectVisible(0, false)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->text_objects.size());
    ASSERT_EQ(500, game_mngr->get_level()->text_objects.at(0).x);
    ASSERT_EQ(600, game_mngr->get_level()->text_objects.at(0).y);
    ASSERT_EQ("SOME_TEXT", game_mngr->get_level()->text_objects.at(0).text_string);
    ASSERT_EQ(ST::hash_string("SOME_FONT.ttf 40"), game_mngr->get_level()->text_objects.at(0).font);
    ASSERT_FALSE(game_mngr->get_level()->text_objects.at(0).is_visible);

    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.r);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.g);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.b);
    ASSERT_EQ(255, game_mngr->get_level()->text_objects.at(0).color.a);
}

TEST_F(lua_backend_test, test_call_function_createEntity){
    test_subject.run_script("createEntity(0)");

    //Check results
    ASSERT_EQ(1, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityActive){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityActive(0, false)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityX){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityX(0, 500)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityY){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityY(0, 500)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityX){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).x = 500;

    //Test
    test_subject.run_script("return getEntityX(0)");

    //Check result
    ASSERT_EQ(500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityY){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).y = 500;

    //Test
    test_subject.run_script("return getEntityY(0)");

    //Check result
    ASSERT_EQ(500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityStatic){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityStatic(0, true)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityVelocityX){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityVelocityX(0, 120)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(120, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityVelocityY){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityVelocityY(0, 120)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(120, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityVelocityX){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).velocity_x = 120;

    //Test
    test_subject.run_script("return getEntityVelocityX(0)");

    //Check result
    ASSERT_EQ(120, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(120, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityVelocityY){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).velocity_y = 120;

    //Test
    test_subject.run_script("return getEntityVelocityY(0)");

    //Check result
    ASSERT_EQ(120, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(120, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityTextureScale){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityTextureScale(0, 2, 2)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(2, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(2, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityTexture){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityTexture(0, \"new_texture.webp\")");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(ST::hash_string("new_texture.webp"), game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityTexW){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityTexW(0, 500)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityTexH){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityTexH(0, 500)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityTexW){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).tex_w = 500;

    //Test
    test_subject.run_script("return getEntityTexW(0)");

    //Check result
    ASSERT_EQ(500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityTexH){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).tex_h = 500;

    //Test
    test_subject.run_script("return getEntityTexH(0)");

    //Check result
    ASSERT_EQ(500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityVisible){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityVisible(0, false)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityCollisionBox){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityCollisionBox(0, 0, 0, 500, 500)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(-500, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_entityCollides){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).x = 500;
    game_mngr->get_level()->entities.at(0).y = 500;
    game_mngr->get_level()->entities.at(1).x = 500;
    game_mngr->get_level()->entities.at(1).y = 500;
    game_mngr->get_level()->entities.at(0).set_collision_box(0, 0, 500, 500);
    game_mngr->get_level()->entities.at(1).set_collision_box(0, 0, 500, 500);

    //Test
    test_subject.run_script("return entityCollides(0, 1)");

    //Check that they are colliding
    ASSERT_TRUE(lua_toboolean(get_lua_state(), lua_gettop(get_lua_state())));

    ASSERT_EQ(2, game_mngr->get_level()->entities.size());

    //Check nothing has been modified in entity 1
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(-500, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());

    //Check nothing has been modified in entity 2
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(1).x);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(1).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(1).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(1).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(1).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(1).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).animation);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(1).get_col_x());
    ASSERT_EQ(-500, game_mngr->get_level()->entities.at(1).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(1).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(1).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityAffectedByPhysics){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityAffectedByPhysics(0, true)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityColX){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).set_collision_box(0, 0, 500, 500);

    //Test
    test_subject.run_script("return getEntityColX(0)");

    //Check result
    ASSERT_EQ(500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(-500, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityColY){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).set_collision_box(0, 0, 500, 500);

    //Test
    test_subject.run_script("return getEntityColY(0)");

    //Check result
    ASSERT_EQ(-500, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(500, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(-500, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityColXOffset){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).set_collision_box(6, 0, 0, 0);

    //Test
    test_subject.run_script("return getEntityColXOffset(0)");

    //Check result
    ASSERT_EQ(6, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(6, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_getEntityColYOffset){
    //Set up
    game_mngr->get_level()->entities.emplace_back();
    game_mngr->get_level()->entities.at(0).set_collision_box(0, 6, 0, 0);

    //Test
    test_subject.run_script("return getEntityColYOffset(0)");

    //Check result
    ASSERT_EQ(6, lua_tointeger(get_lua_state(), lua_gettop(get_lua_state())));

    //Check that nothing has changed in the entity
    ASSERT_EQ(3, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(6, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityAnimation){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityAnimation(0, 3)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(3, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntityAnimationNum){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntityAnimationNum(0, 3)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(3, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

TEST_F(lua_backend_test, test_call_function_setEntitySpriteNum){
    //Set up
    game_mngr->get_level()->entities.emplace_back();

    //Test
    test_subject.run_script("setEntitySpriteNum(0, 3)");

    //Check results
    ASSERT_EQ(2, game_mngr->get_level_calls);
    ASSERT_EQ(1, game_mngr->get_level()->entities.size());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).y);
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_visible());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_static());
    ASSERT_FALSE(game_mngr->get_level()->entities.at(0).is_affected_by_physics());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_y);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).velocity_x);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_h);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).tex_w);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_x);
    ASSERT_EQ(1, game_mngr->get_level()->entities.at(0).tex_scale_y);
    ASSERT_EQ(65535, game_mngr->get_level()->entities.at(0).texture);
    ASSERT_EQ(3, game_mngr->get_level()->entities.at(0).sprite_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation_num);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).animation);
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_x_offset());
    ASSERT_EQ(0, game_mngr->get_level()->entities.at(0).get_col_y_offset());
    ASSERT_TRUE(game_mngr->get_level()->entities.at(0).is_active());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}