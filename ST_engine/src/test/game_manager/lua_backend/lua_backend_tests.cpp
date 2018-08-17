/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <game_manager/lua_backend/lua_backend.hpp>

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

    void SetUp() override{
        test_subject.initialize(new message_bus, nullptr);
    }

    void TearDown() override{
        test_subject.close();
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
