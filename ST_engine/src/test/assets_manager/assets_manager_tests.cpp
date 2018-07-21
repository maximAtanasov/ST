/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <assets_manager/assets_manager.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the assets_manager
class asset_manager_test : public ::testing::Test {

protected:

    void update_task(void* arg){
        assets_manager::update_task(arg);
    }

    ST::assets get_assets(){
        return test_mngr->all_assets;
    }

    std::hash<std::string> hash_f;
    message_bus msg_bus{};
    assets_manager* test_mngr{};

    void SetUp() override{
        initialize_SDL();
        test_mngr = new assets_manager(&msg_bus, nullptr);
    }

    void TearDown() override{
        delete test_mngr;
        close_SDL();
    }
};

TEST_F(asset_manager_test, loadPNG_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.png")));
    update_task(test_mngr);
    ASSERT_FALSE(get_assets().surfaces[hash_f("nothing.png")]);
}

TEST_F(asset_manager_test, loadPNG) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_image_1.png")));
    update_task(test_mngr);
    SDL_Surface* test_surface = IMG_Load("test_image_1.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image_1.png")]));
    SDL_FreeSurface(test_surface);
}

TEST_F(asset_manager_test, loadWAV_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.wav")));
    update_task(test_mngr);
    ASSERT_FALSE(get_assets().chunks[(hash_f("nothing.wav"))]);
}

TEST_F(asset_manager_test, loadWAV) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_sound.wav")));
    update_task(test_mngr);
    Mix_Chunk *expected_chunk = Mix_LoadWAV("test_sound.wav");
    Mix_Chunk *result_chunk = get_assets().chunks[hash_f("test_sound.wav")];
    ASSERT_TRUE(expected_chunk);
    ASSERT_TRUE(result_chunk);
    ASSERT_EQ(expected_chunk->alen, result_chunk->alen);
    for (Uint32 i = 0; i < expected_chunk->alen; i++) {
        ASSERT_EQ(expected_chunk->abuf[i], result_chunk->abuf[i]);
    }
    Mix_FreeChunk(expected_chunk);
}

TEST_F(asset_manager_test, loadOGG_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.ogg")));
    update_task(test_mngr);
    ASSERT_FALSE(get_assets().music[hash_f("nothing.ogg")]);
}

TEST_F(asset_manager_test, loadOGG) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_music.ogg")));
    update_task(test_mngr);
    auto result_music = get_assets().music[hash_f("test_music.ogg")];
    if(result_music == nullptr){
        const char* s = Mix_GetError();
        printf("%s\n", s);
    }
    ASSERT_TRUE(result_music);
    ASSERT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
}

TEST_F(asset_manager_test, loadTTF_nonExistant) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.ttf 50")));
    update_task(test_mngr);
    ASSERT_FALSE(get_assets().fonts["test_font.ttf"]);
}

TEST_F(asset_manager_test, loadTTF_noSize) {
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_font.ttf")));
    update_task(test_mngr);
    ASSERT_FALSE(get_assets().fonts["test_font.ttf"]);
}

TEST_F(asset_manager_test, loadTTF){
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_font.ttf 50")));
    update_task(test_mngr);
    TTF_Font* expected_font = TTF_OpenFont("test_font.ttf", 50);
    TTF_Font* result_font = get_assets().fonts["test_font.ttf50"];
    ASSERT_TRUE(expected_font);
    ASSERT_TRUE(result_font);
    SDL_Surface* expected_render = TTF_RenderText_Blended(result_font, "TEST", {200,200,200,255});
    SDL_Surface* result_render = TTF_RenderText_Blended(result_font, "TEST", {200,200,200,255});
    ASSERT_TRUE(compare_surfaces(expected_render, result_render));
    TTF_CloseFont(expected_font);
    SDL_FreeSurface(expected_render);
    SDL_FreeSurface(result_render);
}

TEST_F(asset_manager_test, loadBinary_PNG) {
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_png.bin")));
    update_task(test_mngr);
    SDL_Surface* test_surface = IMG_Load("test_image_1.png");
    ASSERT_TRUE(test_surface);
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image.png")]));
    SDL_FreeSurface(test_surface);
}

TEST_F(asset_manager_test, loadBinary_WAV) {
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_wav.bin")));
    update_task(test_mngr);
    Mix_Chunk* expected_chunk = Mix_LoadWAV("test_sound.wav");
    Mix_Chunk* result_chunk = get_assets().chunks[hash_f("test_sound.wav")];
    ASSERT_TRUE(expected_chunk);
    ASSERT_TRUE(result_chunk);
    ASSERT_EQ(expected_chunk->alen, result_chunk->alen);
    for(Uint32 i = 0; i < expected_chunk->alen; i++){
        ASSERT_EQ(expected_chunk->abuf[i], result_chunk->abuf[i]);
    }
    Mix_FreeChunk(expected_chunk);
}

TEST_F(asset_manager_test, loadBinary_OGG) {
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_ogg.bin")));
    update_task(test_mngr);
    auto result_music = get_assets().music[hash_f("test_music.ogg")];
    ASSERT_TRUE(result_music);
    ASSERT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
}

TEST_F(asset_manager_test, loadBinary_complex) {
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_complex.bin")));
    update_task(test_mngr);

    //Test music_1
    auto result_music_1 = get_assets().music[hash_f("test_music_1.ogg")];
    ASSERT_TRUE(result_music_1);
    ASSERT_EQ(MUS_OGG, Mix_GetMusicType(result_music_1));

    //Test music_2
    auto result_music_2 = get_assets().music[hash_f("test_music_2.ogg")];
    ASSERT_TRUE(result_music_2);
    ASSERT_EQ(MUS_OGG, Mix_GetMusicType(result_music_2));

    //Test sound_1
    Mix_Chunk* expected_chunk_1 = Mix_LoadWAV("test_sound_1.wav");
    Mix_Chunk* result_chunk_1 = static_cast<Mix_Chunk*>(get_assets().chunks[hash_f("test_sound_1.wav")]);
    ASSERT_TRUE(expected_chunk_1);
    ASSERT_TRUE(result_chunk_1);
    ASSERT_EQ(expected_chunk_1->alen, result_chunk_1->alen);
    for(Uint32 i = 0; i < expected_chunk_1->alen; i++){
        ASSERT_EQ(expected_chunk_1->abuf[i], result_chunk_1->abuf[i]);
    }
    Mix_FreeChunk(expected_chunk_1);

    //Test sound_2
    Mix_Chunk* expected_chunk_2 = Mix_LoadWAV("test_sound_2.wav");
    Mix_Chunk* result_chunk_2 = static_cast<Mix_Chunk*>(get_assets().chunks[hash_f("test_sound_2.wav")]);
    ASSERT_TRUE(expected_chunk_2);
    ASSERT_TRUE(result_chunk_2);
    ASSERT_EQ(expected_chunk_2->alen, result_chunk_2->alen);
    for(Uint32 i = 0; i < expected_chunk_2->alen; i++){
        ASSERT_EQ(expected_chunk_2->abuf[i], result_chunk_2->abuf[i]);
    }
    Mix_FreeChunk(expected_chunk_2);

    //Test image_1
    SDL_Surface* test_surface_1 = IMG_Load("test_image_1.png");
    ASSERT_TRUE(test_surface_1);
    ASSERT_TRUE(compare_surfaces(test_surface_1, get_assets().surfaces[hash_f("test_image_1.png")]));
    SDL_FreeSurface(test_surface_1);

    //Test image_2
    SDL_Surface* test_surface_2 = IMG_Load("test_image_2.png");
    ASSERT_TRUE(test_surface_2);
    ASSERT_TRUE(compare_surfaces(test_surface_2, get_assets().surfaces[hash_f("test_image_2.png")]));
    SDL_FreeSurface(test_surface_2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}