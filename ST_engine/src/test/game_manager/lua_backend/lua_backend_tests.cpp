/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#define TESTING_LUA_BACKEND

#include <gtest/gtest.h>
#include <game_manager/lua_backend/lua_backend.cpp>
#include <main/timer.hpp>

/// Tests fixture for the lua_backend
class lua_backend_test : public ::testing::Test {

protected:
    lua_backend test_subject{};

    std::string hash_file(std::string path){
        return test_subject.hash_file(path);
    }

    std::string hash_string(std::string string){
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
        FAIL();
    }catch (const std::out_of_range& e){
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
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyHeld){
    std::string result = hash_file("lua_scripts/test_script_keyHeld.lua");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyPressed){
    std::string result = hash_file("lua_scripts/test_script_keyPressed.lua");
    std::string resulting_integer = result.substr(11, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyReleased){
    std::string result = hash_file("lua_scripts/test_script_keyReleased.lua");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
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
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_keyAnnotation){
    std::string result = hash_file("lua_scripts/test_script_keyAnnotation.lua");
    std::string resulting_integer = result.substr(10, std::string::npos);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_file_audioAnnotation){
    std::string result = hash_file("lua_scripts/test_script_audioAnnotation.lua");
    std::string resulting_integer = result.substr(12, std::string::npos);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_playSound){
    std::string result = hash_string("playSound(\"sound.wav\", 100, 1)");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-10);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_playMusic){
    std::string result = hash_string("playMusic(\"music.mp3\", 100, 1)");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-10);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_keyHeld){
    std::string result = hash_string("keyHeld(\"JUMP\")");
    std::string resulting_integer = result.substr(10, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_keyPressed){
    std::string result = hash_string("keyPressed(\"JUMP\")");
    std::string resulting_integer = result.substr(11, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_keyReleased){
    std::string result = hash_string("keyReleased(\"JUMP\")");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
        FAIL();
    }
}

TEST_F(lua_backend_test, test_hash_string_setClickKey){
    std::string result = hash_string("setClickKey(\"MOUSE1\")");
    std::string resulting_integer = result.substr(12, std::string::npos);
    resulting_integer.erase(resulting_integer.size()-3);
    try{
        std::stoull(resulting_integer);
    }catch(const std::invalid_argument& e) {
        FAIL();
    }catch (const std::out_of_range& e){
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
    ASSERT_EXIT(test_subject.run_file("lua_scripts/test_script_broken.lua"), ::testing::KilledBySignal(SIGABRT), ".*");
}

TEST_F(lua_backend_test, test_call_function_setFullscreen){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_FULLSCREEN, &subscriber1);

    //Test
    test_subject.run_script("setFullscreen(true)");


    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_FULLSCREEN, result->msg_name);
    ASSERT_TRUE(*static_cast<bool*>(result->get_data()));
}

TEST_F(lua_backend_test, test_call_function_hashString){
    //Set up
    std::string test_string = "TEST_STRING";
    std::hash<std::string> hash_f;
    size_t expected = hash_f(test_string);

    //Test
    test_subject.run_script("return hashString(\"" + test_string +"\")");
    ASSERT_EQ(expected, lua_tointeger(get_lua_state(), -1));
}

TEST_F(lua_backend_test, test_call_function_delay){
    //Set up
    timer test_timer;
    uint64_t runtime = 500;

    //Test
    double start_time = test_timer.time_since_start();
    test_subject.run_script("delay(" + std::to_string(runtime) + ")");
    double end_time = test_timer.time_since_start();

    ASSERT_NEAR(runtime, static_cast<uint32_t>(end_time-start_time), 20);
}

TEST_F(lua_backend_test, test_call_function_vsyncOn){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(VSYNC_ON, &subscriber1);

    //Test
    test_subject.run_script("vsyncOn()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(VSYNC_ON, result->msg_name);
    ASSERT_FALSE(result->get_data());
}

TEST_F(lua_backend_test, test_call_function_vsyncOff){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(VSYNC_OFF, &subscriber1);

    //Test
    test_subject.run_script("vsyncOff()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(VSYNC_OFF, result->msg_name);
    ASSERT_FALSE(result->get_data());
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
    ASSERT_TRUE(*static_cast<bool*>(result->get_data()));
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
    ASSERT_FALSE(*static_cast<bool*>(result->get_data()));
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
    ASSERT_EQ(500, *static_cast<int32_t*>(result->get_data()));
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
    ASSERT_EQ(12, *static_cast<uint8_t*>(result->get_data()));
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
    std::hash<std::string> hash_f;
    size_t expected = hash_f(test_string);

    //Test
    test_subject.run_script("playSound(\"" + test_string + "\", 100, 3)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();
    ASSERT_TRUE(result);
    ASSERT_EQ(PLAY_SOUND, result->msg_name);

    auto data = static_cast<std::tuple<size_t, uint8_t, int8_t>*>(result->get_data());
    size_t name = std::get<0> (*data);
    uint8_t volume = std::get<1> (*data);
    int8_t loops = std::get<2> (*data);

    ASSERT_EQ(expected, name);
    ASSERT_EQ(100, volume);
    ASSERT_EQ(3, loops);
}

TEST_F(lua_backend_test, test_call_function_playMusic){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(PLAY_MUSIC, &subscriber1);
    std::string test_string = "some_music.mp3";
    std::hash<std::string> hash_f;
    size_t expected = hash_f(test_string);

    //Test
    test_subject.run_script("playMusic(\"" + test_string + "\", 128, 2)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();
    ASSERT_TRUE(result);
    ASSERT_EQ(PLAY_MUSIC, result->msg_name);

    auto data = static_cast<std::tuple<size_t, uint8_t, int8_t>*>(result->get_data());
    size_t name = std::get<0> (*data);
    uint8_t volume = std::get<1> (*data);
    int8_t loops = std::get<2> (*data);

    ASSERT_EQ(expected, name);
    ASSERT_EQ(128, volume);
    ASSERT_EQ(2, loops);
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

TEST_F(lua_backend_test, test_call_function_toggleAudio){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(TOGGLE_AUDIO, &subscriber1);

    //Test
    test_subject.run_script("toggleAudio()");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(TOGGLE_AUDIO, result->msg_name);
    ASSERT_FALSE(result->get_data());
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

TEST_F(lua_backend_test, test_call_function_setVolume){
    //Set up
    subscriber subscriber1;
    msg_bus->subscribe(SET_VOLUME, &subscriber1);

    //Test
    test_subject.run_script("setVolume(50)");

    //Check result - expect to see a message with appropriate content
    message* result = subscriber1.get_next_message();

    ASSERT_TRUE(result);
    ASSERT_EQ(SET_VOLUME, result->msg_name);
    ASSERT_EQ(50, *static_cast<uint8_t*>(result->get_data()));
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
    ASSERT_EQ(250, *static_cast<uint8_t*>(result->get_data()));
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
    ASSERT_FALSE(*static_cast<bool*>(result->get_data()));
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
    ASSERT_EQ(1, game_mngr->get_level_data_calls);
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
    ASSERT_EQ(2, game_mngr->get_level_data_calls);
}

TEST_F(lua_backend_test, test_call_function_getVolume){
    game_mngr->volume_level = 123;
    test_subject.run_script("return getVolume()");

    //Check result
    ASSERT_EQ(123, lua_tointeger(get_lua_state(), -1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
