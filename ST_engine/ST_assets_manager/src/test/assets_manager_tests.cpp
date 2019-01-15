/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <gtest/gtest.h>
#include <assets_manager.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the assets_manager
class asset_manager_test : public ::testing::Test {

protected:

    ST::assets get_assets(){
        return test_mngr->all_assets;
    }

    int8_t load_asset(const std::string& path){
        return test_mngr->load_asset(path);
    }

    int8_t unload_asset(const std::string& path){
        return test_mngr->unload_asset(path);
    }

    int8_t load_assets_from_binary(const std::string& path){
        return test_mngr->load_assets_from_binary(path);
    }

    int8_t load_assets_from_list(const std::string& path){
        return test_mngr->load_assets_from_list(path);
    }

    int8_t unload_assets_from_list(const std::string& path){
        return test_mngr->unload_assets_from_list(path);
    }

    uint16_t get_count(const std::string& asset_name){
        return test_mngr->count[asset_name];
    };

    std::hash<std::string> hash_f;
    assets_manager* test_mngr{};

    static void SetUpTestCase(){
        initialize_SDL();
    }

    static void TearDownTestCase(){
        close_SDL();
    }

    message_bus* msg_bus{};

    void SetUp() override{
        msg_bus = new message_bus();
        test_mngr = new assets_manager(msg_bus, nullptr);
    }

    void TearDown() override{
        delete test_mngr;
        delete msg_bus;
    }
};

TEST_F(asset_manager_test, loadPNG_nonExistant) {
    load_asset("nothing.png");
    ASSERT_FALSE(get_assets().surfaces[hash_f("nothing.png")]);
}

TEST_F(asset_manager_test, loadPNG) {
    load_asset("test_image_1.png");
    SDL_Surface* test_surface = IMG_Load("test_image_1.png");
    ASSERT_TRUE(test_surface);
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image_1.png")]));
    SDL_FreeSurface(test_surface);
}

TEST_F(asset_manager_test, loadWEBP_nonExistant) {
    load_asset("nothing.webp");
    ASSERT_FALSE(get_assets().surfaces[hash_f("nothing.webp")]);
}

TEST_F(asset_manager_test, loadWEBP) {
    load_asset("test_image_3.webp");
    SDL_Surface* test_surface = IMG_Load("test_image_3.webp");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image_3.webp")]));
    SDL_FreeSurface(test_surface);
}

TEST_F(asset_manager_test, loadWAV_nonExistant) {
    load_asset("nothing.wav");
    ASSERT_FALSE(get_assets().chunks[(hash_f("nothing.wav"))]);
}

TEST_F(asset_manager_test, loadWAV) {
    load_asset("test_sound.wav");
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
    ASSERT_EQ(-1, load_asset("nothing.ogg"));
    ASSERT_FALSE(get_assets().music[hash_f("nothing.ogg")]);
}

TEST_F(asset_manager_test, loadOGG) {
    ASSERT_EQ(0, load_asset("test_music.ogg"));
    auto result_music = get_assets().music[hash_f("test_music.ogg")];
    if(result_music == nullptr){
        const char* s = Mix_GetError();
        printf("%s\n", s);
    }
    ASSERT_TRUE(result_music);
    ASSERT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
}

TEST_F(asset_manager_test, loadTTF_nonExistant) {
    ASSERT_EQ(-1, load_asset("nothing.ttf 50"));
    ASSERT_FALSE(get_assets().fonts["nothing.ttf 50"]);
}

TEST_F(asset_manager_test, loadTTF_noSize) {
    ASSERT_EQ(-1, load_asset("test_font.ttf"));
    ASSERT_FALSE(get_assets().fonts["test_font.ttf"]);
}

TEST_F(asset_manager_test, loadTTF){
    load_asset("test_font.ttf 50");
    TTF_Font* expected_font = TTF_OpenFont("test_font.ttf", 50);
    TTF_Font* result_font = get_assets().fonts["test_font.ttf 50"];
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
    ASSERT_EQ(0, load_assets_from_binary("test_binary_png.bin"));
    SDL_Surface* test_surface = IMG_Load("test_image_1.png");
    ASSERT_TRUE(test_surface);
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image.png")]));
    SDL_FreeSurface(test_surface);
}

TEST_F(asset_manager_test, loadBinary_WEBP) {
    ASSERT_EQ(0, load_assets_from_binary("test_binary_webp.bin"));
    SDL_Surface* test_surface = IMG_Load("test_image_3.webp");
    ASSERT_TRUE(test_surface);
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image_3.webp")]));
    SDL_FreeSurface(test_surface);
}

TEST_F(asset_manager_test, loadBinary_WAV) {
    ASSERT_EQ(0, load_assets_from_binary("test_binary_wav.bin"));
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
    ASSERT_EQ(0, load_assets_from_binary("test_binary_ogg.bin"));
    auto result_music = get_assets().music[hash_f("test_music.ogg")];
    ASSERT_TRUE(result_music);
    ASSERT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
}

TEST_F(asset_manager_test, loadBinary_complex) {
    ASSERT_EQ(0, load_assets_from_binary("test_binary_complex.bin"));

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

    //Test image_3
    SDL_Surface* test_surface_3 = IMG_Load("test_image_3.webp");
    ASSERT_TRUE(test_surface_3);
    ASSERT_TRUE(compare_surfaces(test_surface_3, get_assets().surfaces[hash_f("test_image_3.webp")]));
    SDL_FreeSurface(test_surface_3);
}

TEST_F(asset_manager_test, test_load_assets_from_list){

    ASSERT_EQ(0, load_assets_from_list("test_list_1.list"));

    //Test image_1
    SDL_Surface* test_surface_1 = IMG_Load("test_image_1.png");
    ASSERT_TRUE(test_surface_1);
    ASSERT_TRUE(compare_surfaces(test_surface_1, get_assets().surfaces[hash_f("test_image_1.png")]));
    SDL_FreeSurface(test_surface_1);

    //Test image_2
    SDL_Surface* test_surface_2 = IMG_Load("test_sprite.png");
    ASSERT_TRUE(test_surface_2);
    ASSERT_TRUE(compare_surfaces(test_surface_2, get_assets().surfaces[hash_f("test_sprite.png")]));
    SDL_FreeSurface(test_surface_2);

    //Test image_3
    SDL_Surface* test_surface_3 = IMG_Load("test_image_3.webp");
    ASSERT_TRUE(test_surface_3);
    ASSERT_TRUE(compare_surfaces(test_surface_3, get_assets().surfaces[hash_f("test_image_3.webp")]));
    SDL_FreeSurface(test_surface_3);

    //Test sound_1
    Mix_Chunk* expected_chunk_1 = Mix_LoadWAV("test_sound.wav");
    Mix_Chunk* result_chunk_1 = static_cast<Mix_Chunk*>(get_assets().chunks[hash_f("test_sound.wav")]);
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
}

TEST_F(asset_manager_test, test_fail_when_list_does_not_exist){
    ASSERT_EQ(-1, load_assets_from_list("no_list.list"));
}


TEST_F(asset_manager_test, test_load_asset_twice){

    ASSERT_EQ(0, load_asset("test_image_1.png"));
    ASSERT_EQ(0, load_asset("test_image_1.png"));

    ASSERT_EQ(2, get_count("test_image_1.png"));

    SDL_Surface* test_surface = IMG_Load("test_image_1.png");

    ASSERT_TRUE(static_cast<bool>(test_surface));
    ASSERT_TRUE(compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image_1.png")]));

    SDL_FreeSurface(test_surface);
}


TEST_F(asset_manager_test, test_load_and_unload_asset){
    ASSERT_EQ(0, load_asset("test_image_1.png"));
    ASSERT_EQ(1, get_count("test_image_1.png"));
    ASSERT_EQ(0, unload_asset("test_image_1.png"));
    ASSERT_EQ(0, get_count("test_image_1.png"));
    ASSERT_FALSE(get_assets().surfaces[hash_f("test_image_1.png")]);
}

TEST_F(asset_manager_test, test_load_twice_and_unload_asset){
    ASSERT_EQ(0, load_asset("test_image_1.png"));
    ASSERT_EQ(0, load_asset("test_image_1.png"));
    ASSERT_EQ(2, get_count("test_image_1.png"));
    ASSERT_EQ(0, unload_asset("test_image_1.png"));
    ASSERT_EQ(1, get_count("test_image_1.png"));
    ASSERT_TRUE(get_assets().surfaces[hash_f("test_image_1.png")]);
}

TEST_F(asset_manager_test, test_load_and_unload_assets_from_list){
    ASSERT_EQ(0, load_assets_from_list("test_list_1.list"));

    ASSERT_EQ(1, get_count("test_image_1.png"));
    ASSERT_EQ(1, get_count("test_sound.wav"));
    ASSERT_EQ(1, get_count("test_sprite.png"));
    ASSERT_EQ(1, get_count("test_sound_2.wav"));

    ASSERT_EQ(0, unload_assets_from_list("test_list_1.list"));

    ASSERT_EQ(0, get_count("test_image_1.png"));
    ASSERT_EQ(0, get_count("test_sound.wav"));
    ASSERT_EQ(0, get_count("test_sprite.png"));
    ASSERT_EQ(0, get_count("test_sound_2.wav"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}