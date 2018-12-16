/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#define TESTING

#include <gtest/gtest.h>
#include <main/main.cpp>
#include <thread>
#include <future>

/*
 * These integration tests will start the engine and the available ST levels.
 * We use the message bus and the dev console to control the gameplay and test different commands.
 * When running these tests manually, unwanted/wrong behaviour can be noticed. When running in a
 * CI environment, these tests will tell if the engine runs and if basic functionality causes a crash.
 */

int start_engine_thread(){
    auto args = static_cast<char**>(malloc(10));
    args[0] = const_cast<char*>("ST_engine");
    return ST_engine_main(1, args);
}

void basic_run(){
    SDL_Delay(25000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
    SDL_Delay(2000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::TILDE)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::TILDE)));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("endGame()")));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));

}

void set_vsync(){
    SDL_Delay(25000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::TILDE)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::TILDE)));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("setVsync(false)")));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("setVsync(true)")));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("endGame()")));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
}

void set_fullscreen(){
    SDL_Delay(25000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::TILDE)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::TILDE)));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("setFullscreen(false)")));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("setFullscreen(true)")));
    SDL_Delay(3000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("endGame()")));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
}

void reload_and_restart(){
    SDL_Delay(25000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(2000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::TILDE)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::TILDE)));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>(R"(loadLevel("main"); startLevel("main"))")));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
    SDL_Delay(25000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
    SDL_Delay(2000);
    gMessage_bus.send_msg(make_msg(TEXT_STREAM, make_data<std::string>("endGame()")));
    SDL_Delay(1000);
    gMessage_bus.send_msg(make_msg(KEY_PRESSED, make_data(ST::key::ENTER)));
    SDL_Delay(30);
    gMessage_bus.send_msg(make_msg(KEY_RELEASED, make_data(ST::key::ENTER)));
}

TEST(engine_integration, full_integration) {
    gMessage_bus.clear();
    auto engine_thread = std::async(start_engine_thread);
    basic_run();
    ASSERT_EQ(0, engine_thread.get());
}

TEST(engine_integration, full_integration_set_vsync) {
    gMessage_bus.clear();
    auto engine_thread = std::async(start_engine_thread);
    set_vsync();
    ASSERT_EQ(0, engine_thread.get());
}

TEST(engine_integration, full_integration_set_fullscreen) {
    gMessage_bus.clear();
    auto engine_thread = std::async(start_engine_thread);
    set_fullscreen();
    ASSERT_EQ(0, engine_thread.get());
}

TEST(engine_integration, full_integration_reload_and_restart) {
    gMessage_bus.clear();
    auto engine_thread = std::async(start_engine_thread);
    reload_and_restart();
    ASSERT_EQ(0, engine_thread.get());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}