#include <gtest/gtest.h>
#include <assets_manager/assets_manager.hpp>
#include <test_util.hpp>

/// Tests fixture for the assets_manager
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
    }

    void TearDown() override{
        msg_bus.close();
        close_SDL();
    }

    std::hash<std::string> hash_f;
    message_bus msg_bus{};
    assets_manager test_mngr{};
};

TEST_F(asset_manager_test, loadPNG_nonExistant) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.png")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().surfaces[hash_f("nothing.png")]);
    test_mngr.close();
}

TEST_F(asset_manager_test, loadPNG) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_image.png")));
    update_task(&test_mngr);
    SDL_Surface* test_surface = IMG_Load("test_image.png");
    if(test_surface == nullptr){
        const char* s = IMG_GetError();
        printf("%s\n", s);
    }
    ASSERT_EQ(true, static_cast<bool>(test_surface));
    EXPECT_EQ(true, compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image.png")]));
    test_mngr.close();
}

TEST_F(asset_manager_test, loadWAV_nonExistant) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.wav")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().chunks.get(hash_f("nothing.wav")));
    test_mngr.close();
}

TEST_F(asset_manager_test, loadWAV) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_sound.wav")));
    update_task(&test_mngr);
    Mix_Chunk* expected_chunk = Mix_LoadWAV("test_sound.wav");
    Mix_Chunk* result_chunk = get_assets().chunks.get(hash_f("test_sound.wav"));
    ASSERT_EQ(true, static_cast<bool>(expected_chunk));
    ASSERT_EQ(true, static_cast<bool>(result_chunk));
    ASSERT_EQ(expected_chunk->alen, result_chunk->alen);
    for(Uint32 i = 0; i < expected_chunk->alen; i++){
        ASSERT_EQ(expected_chunk->abuf[i], result_chunk->abuf[i]);
    }
    test_mngr.close();
}

TEST_F(asset_manager_test, loadOGG_nonExistant) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.ogg")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().music[hash_f("nothing.ogg")]);
    test_mngr.close();
}

TEST_F(asset_manager_test, loadOGG) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_music.ogg")));
    update_task(&test_mngr);
    Mix_Music* result_music = get_assets().music[hash_f("test_music.ogg")];
    if(result_music == nullptr){
        const char* s = Mix_GetError();
        printf("%s\n", s);
    }
    ASSERT_EQ(true, static_cast<bool>(result_music));
    EXPECT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
    test_mngr.close();
}

TEST_F(asset_manager_test, loadTTF_nonExistant) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("nothing.ttf 50")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().fonts["test_font.ttf"]);
    test_mngr.close();
}

TEST_F(asset_manager_test, loadTTF_noSize) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_font.ttf")));
    update_task(&test_mngr);
    EXPECT_EQ(nullptr, get_assets().fonts["test_font.ttf"]);
    test_mngr.close();
}

TEST_F(asset_manager_test, loadTTF){
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_ASSET, make_data<std::string>("test_font.ttf 50")));
    update_task(&test_mngr);
    TTF_Font* expected_font = TTF_OpenFont("test_font.ttf", 50);
    TTF_Font* result_font = get_assets().fonts["test_font.ttf50"];
    ASSERT_EQ(true, static_cast<bool>(expected_font));
    ASSERT_EQ(true, static_cast<bool>(result_font));
    SDL_Surface* expected_render = TTF_RenderText_Blended(result_font, "TEST", {200,200,200,255});
    SDL_Surface* result_render = TTF_RenderText_Blended(result_font, "TEST", {200,200,200,255});
    EXPECT_EQ(true, compare_surfaces(expected_render, result_render));
    test_mngr.close();
}

TEST_F(asset_manager_test, loadBinary_PNG) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_png.bin")));
    update_task(&test_mngr);
    SDL_Surface* test_surface = IMG_Load("test_image.png");
    ASSERT_EQ(true, static_cast<bool>(test_surface));
    EXPECT_EQ(true, compare_surfaces(test_surface, get_assets().surfaces[hash_f("test_image.png")]));
    test_mngr.close();
}

TEST_F(asset_manager_test, loadBinary_WAV) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_wav.bin")));
    update_task(&test_mngr);
    Mix_Chunk* expected_chunk = Mix_LoadWAV("test_sound.wav");
    Mix_Chunk* result_chunk = get_assets().chunks.get(hash_f("test_sound.wav"));
    ASSERT_EQ(true, static_cast<bool>(expected_chunk));
    ASSERT_EQ(true, static_cast<bool>(result_chunk));
    ASSERT_EQ(expected_chunk->alen, result_chunk->alen);
    for(Uint32 i = 0; i < expected_chunk->alen; i++){
        ASSERT_EQ(expected_chunk->abuf[i], result_chunk->abuf[i]);
    }
    test_mngr.close();
}

TEST_F(asset_manager_test, loadBinary_OGG) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_ogg.bin")));
    update_task(&test_mngr);
    auto result_music = get_assets().music[hash_f("test_music.ogg")];
    ASSERT_EQ(true, static_cast<bool>(result_music));
    EXPECT_EQ(MUS_OGG, Mix_GetMusicType(result_music));
    test_mngr.close();
}

TEST_F(asset_manager_test, loadBinary_complex) {
    test_mngr.initialize(&msg_bus, nullptr);
    msg_bus.send_msg(make_msg(LOAD_BINARY, make_data<std::string>("test_binary_complex.bin")));
    update_task(&test_mngr);

    //Test music_1
    auto result_music_1 = get_assets().music[hash_f("test_music_1.ogg")];
    ASSERT_EQ(true, static_cast<bool>(result_music_1));
    EXPECT_EQ(MUS_OGG, Mix_GetMusicType(result_music_1));

    //Test music_2
    auto result_music_2 = get_assets().music[hash_f("test_music_2.ogg")];
    ASSERT_EQ(true, static_cast<bool>(result_music_2));
    EXPECT_EQ(MUS_OGG, Mix_GetMusicType(result_music_2));

    //Test sound_1
    Mix_Chunk* expected_chunk_1 = Mix_LoadWAV("test_sound_1.wav");
    Mix_Chunk* result_chunk_1 = get_assets().chunks.get(hash_f("test_sound_1.wav"));
    ASSERT_EQ(true, static_cast<bool>(expected_chunk_1));
    ASSERT_EQ(true, static_cast<bool>(result_chunk_1));
    ASSERT_EQ(expected_chunk_1->alen, result_chunk_1->alen);
    for(Uint32 i = 0; i < expected_chunk_1->alen; i++){
        ASSERT_EQ(expected_chunk_1->abuf[i], result_chunk_1->abuf[i]);
    }

    //Test sound_2
    Mix_Chunk* expected_chunk_2 = Mix_LoadWAV("test_sound_2.wav");
    Mix_Chunk* result_chunk_2 = get_assets().chunks.get(hash_f("test_sound_2.wav"));
    ASSERT_EQ(true, static_cast<bool>(expected_chunk_2));
    ASSERT_EQ(true, static_cast<bool>(result_chunk_2));
    ASSERT_EQ(expected_chunk_2->alen, result_chunk_2->alen);
    for(Uint32 i = 0; i < expected_chunk_2->alen; i++){
        ASSERT_EQ(expected_chunk_2->abuf[i], result_chunk_2->abuf[i]);
    }

    //Test image_1
    SDL_Surface* test_surface_1 = IMG_Load("test_image_1.png");
    ASSERT_EQ(true, static_cast<bool>(test_surface_1));
    EXPECT_EQ(true, compare_surfaces(test_surface_1, get_assets().surfaces[hash_f("test_image_1.png")]));

    //Test image_2
    SDL_Surface* test_surface_2 = IMG_Load("test_image_2.png");
    ASSERT_EQ(true, static_cast<bool>(test_surface_2));
    EXPECT_EQ(true, compare_surfaces(test_surface_2, get_assets().surfaces[hash_f("test_image_2.png")]));
    test_mngr.close();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}