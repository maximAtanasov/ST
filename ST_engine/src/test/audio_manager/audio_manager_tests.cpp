/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <audio_manager/audio_manager.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the audio_manager
class audio_manager_test : public ::testing::Test {

protected:

    void set_assets(ST::assets* assets){
        test_mngr->assets_ptr = assets;
    }

    void play_sound(size_t arg, uint8_t volume, uint8_t loops){
        test_mngr->play_sound(arg, volume, loops);
    }

    void play_music(size_t arg, uint8_t volume, uint8_t loops){
        test_mngr->play_music(arg, volume, loops);
    }

    audio_manager* test_mngr{};

    static void SetUpTestCase(){
        initialize_SDL();
    }

    static void TearDownTestCase(){
        close_SDL();
    }

    void SetUp() override{
        test_mngr = new audio_manager(new message_bus(), nullptr);
    }

    void TearDown() override{
        delete test_mngr;
    }
};

TEST_F(audio_manager_test, test_play_wav_full_volume) {
    auto test_wav = Mix_LoadWAV("test_sound.wav");
    ASSERT_TRUE(test_wav);
    ST::assets assets;
    assets.chunks[1] = test_wav;
    set_assets(&assets);
    play_sound(1, MIX_MAX_VOLUME, 0);
    SDL_Delay(1000);
    Mix_FreeChunk(test_wav);
}

TEST_F(audio_manager_test, test_play_wav_half_volume) {
    auto test_wav = Mix_LoadWAV("test_sound.wav");
    ASSERT_TRUE(test_wav);
    ST::assets assets;
    assets.chunks[1] = test_wav;
    set_assets(&assets);
    play_sound(1, MIX_MAX_VOLUME/2, 0);
    SDL_Delay(1000);
}

TEST_F(audio_manager_test, test_play_wav_looping) {
    auto test_wav = Mix_LoadWAV("test_sound.wav");
    ASSERT_TRUE(test_wav);
    ST::assets assets;
    assets.chunks[1] = test_wav;
    set_assets(&assets);
    play_sound(1, MIX_MAX_VOLUME, 3);
    SDL_Delay(2000);
}

TEST_F(audio_manager_test, test_play_ogg_full_volume) {
    auto test_ogg = Mix_LoadMUS("test_music.ogg");
    ASSERT_TRUE(test_ogg);
    ST::assets assets;
    assets.music[1] = test_ogg;
    set_assets(&assets);
    play_music(1, MIX_MAX_VOLUME, 3);
    SDL_Delay(10000);
}

TEST_F(audio_manager_test, test_play_ogg_half_volume) {
    auto test_ogg = Mix_LoadMUS("test_music.ogg");
    ASSERT_TRUE(test_ogg);
    ST::assets assets;
    assets.music[1] = test_ogg;
    set_assets(&assets);
    play_music(1, MIX_MAX_VOLUME/2, 3);
    SDL_Delay(10000);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}