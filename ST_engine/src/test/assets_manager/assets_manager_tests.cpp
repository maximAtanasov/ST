#include <gtest/gtest.h>
#include <SDL2/SDL_test_compare.h>
#include "../../main/assets_manager/assets_manager.hpp"
#include "../test_util.hpp"

class asset_manager_test : public ::testing::Test {

public:
    void update_task(void* arg){
        assets_manager::update_task(arg);
    }

    ST::assets get_assets(){
        return test_mngr.all_assets;
    }

protected:
    void SetUp() override{
        initialize_SDL();
        msg_bus.initialize();
        test_mngr.initialize(&msg_bus, nullptr);
    }

    void TearDown() override{
        test_mngr.close();
        msg_bus.close();
        close_SDL();
    }

    std::hash<std::string> hash_f;
    message_bus msg_bus{};
    assets_manager test_mngr{};
};

TEST_F(asset_manager_test, loadPNG_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.png")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().surfaces[hash_f("nothing.png")]);
}

TEST_F(asset_manager_test, loadPNG) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("../../ST_engine/src/test/assets_manager/test_image.png")));
    update_task(&test_mngr);
    SDL_Surface* test_surface = IMG_Load("../../ST_engine/src/test/assets_manager/test_image.png");
    ASSERT_EQ(true, static_cast<bool>(test_surface));
    EXPECT_EQ(0, SDLTest_CompareSurfaces(test_surface, get_assets().surfaces[hash_f("test_image.png")], 0));
}

TEST_F(asset_manager_test, loadWAV_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.wav")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().chunks[hash_f("nothing.png")]);
}

TEST_F(asset_manager_test, loadWAV) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("../../ST_engine/src/test/assets_manager/test_sound.wav")));
    update_task(&test_mngr);
    Mix_Chunk* expected_chunk = Mix_LoadWAV("../../ST_engine/src/test/assets_manager/test_sound.wav");
    Mix_Chunk* result_chunk = get_assets().chunks[hash_f("test_sound.wav")];
    ASSERT_EQ(true, static_cast<bool>(expected_chunk));
    ASSERT_EQ(true, static_cast<bool>(result_chunk));
    ASSERT_EQ(expected_chunk->alen, result_chunk->alen);
    for(Uint32 i = 0; i < expected_chunk->alen; i++){
        EXPECT_EQ(expected_chunk->abuf[i], result_chunk->abuf[i]);
    }
}

TEST_F(asset_manager_test, loadOGG_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.ogg")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().music[hash_f("nothing.ogg")]);
}

TEST_F(asset_manager_test, loadOGG) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("../../ST_engine/src/test/assets_manager/test_music.ogg")));
    update_task(&test_mngr);
    Mix_Music* result_music = get_assets().music[hash_f("test_music.ogg")];
    ASSERT_EQ(true, static_cast<bool>(result_music));
    EXPECT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
}

TEST_F(asset_manager_test, loadTTF_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.ttf 50")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().fonts["test_font.ttf"]);
}

TEST_F(asset_manager_test, loadTTF_noSize) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("../../ST_engine/src/test/assets_manager/test_font.ttf")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().fonts["test_font.ttf"]);
}

TEST_F(asset_manager_test, loadTTF){
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("../../ST_engine/src/test/assets_manager/test_font.ttf 50")));
    update_task(&test_mngr);
    TTF_Font* expected_font = TTF_OpenFont("../../ST_engine/src/test/assets_manager/test_font.ttf", 50);
    TTF_Font* result_font = get_assets().fonts["test_font.ttf50"];
    ASSERT_EQ(true, static_cast<bool>(expected_font));
    ASSERT_EQ(true, static_cast<bool>(result_font));
    SDL_Surface* expected_render = TTF_RenderText_Blended(result_font, "TEST", {200,200,200,255});
    SDL_Surface* result_render = TTF_RenderText_Blended(result_font, "TEST", {200,200,200,255});
    EXPECT_EQ(0, SDLTest_CompareSurfaces(expected_render, result_render, 0));
}

/*TEST_F(asset_manager_test, loadBinary_PNG) {
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("../../ST_engine/src/test/assets_manager/test_binary.bin")));
    update_task(&test_mngr);
    SDL_Surface* test_surface = IMG_Load("../src/test/assets_manager/test_image.png");
    ASSERT_EQ(true, static_cast<bool>(test_surface));
    EXPECT_EQ(0, SDLTest_CompareSurfaces(test_surface, get_assets().surfaces[hash_f("test_image.png")], 0));
}*/

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}