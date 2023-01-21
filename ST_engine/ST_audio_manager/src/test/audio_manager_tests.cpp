/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <gtest/gtest.h>
#include <audio_manager.hpp>

/// Tests fixture for the audio_manager
class audio_manager_test : public ::testing::Test {

protected:

    void set_chunks(ska::bytell_hash_map<uint16_t, Mix_Chunk *> *assets) {
        test_mngr->chunks_ptr = assets;
    }

    void set_music(ska::bytell_hash_map<uint16_t, Mix_Music *> *assets) {
        test_mngr->music_ptr = assets;
    }

    void play_sound(uint16_t arg, uint8_t volume, uint8_t loops) {
        test_mngr->play_sound(arg, volume, loops);
    }

    void play_music(uint16_t arg, uint8_t volume, uint8_t loops) {
        test_mngr->play_music(arg, volume, loops);
    }

    audio_manager *test_mngr{};
    message_bus *msg_bus{};
    task_manager *task_mngr{};

    static void SetUpTestCase() {
        SDL_Init(SDL_INIT_VIDEO);
        Mix_OpenAudio(22050, AUDIO_F32SYS, 2, 1024);
        Mix_Init(MIX_INIT_OGG);
    }

    static void TearDownTestCase() {
        while (Mix_Init(0)) {
            Mix_Quit();
        }
        Mix_CloseAudio();
        SDL_Quit();
    }


    void SetUp() override {
        msg_bus = new message_bus();
        task_mngr = new task_manager(0);
        test_mngr = new audio_manager(*task_mngr, *msg_bus);
    }

    void TearDown() override {
        delete test_mngr;
        delete msg_bus;
        delete task_mngr;
    }
};

TEST_F(audio_manager_test, test_play_wav_full_volume) {
    auto test_wav = Mix_LoadWAV("test_sound.wav");
    ASSERT_TRUE(test_wav);
    ska::bytell_hash_map<uint16_t, Mix_Chunk *> assets;
    assets[1] = test_wav;
    set_chunks(&assets);
    play_sound(1, MIX_MAX_VOLUME, 0);
    SDL_Delay(1000);
    Mix_FreeChunk(test_wav);
}

TEST_F(audio_manager_test, test_play_wav_half_volume) {
    auto test_wav = Mix_LoadWAV("test_sound.wav");
    ASSERT_TRUE(test_wav);
    ska::bytell_hash_map<uint16_t, Mix_Chunk *> assets;
    assets[1] = test_wav;
    set_chunks(&assets);
    play_sound(1, MIX_MAX_VOLUME / 2, 0);
    SDL_Delay(1000);
    Mix_FreeChunk(test_wav);
}

TEST_F(audio_manager_test, test_play_wav_looping) {
    auto test_wav = Mix_LoadWAV("test_sound.wav");
    ASSERT_TRUE(test_wav);
    ska::bytell_hash_map<uint16_t, Mix_Chunk *> assets;
    assets[1] = test_wav;
    set_chunks(&assets);
    play_sound(1, MIX_MAX_VOLUME, 3);
    SDL_Delay(2000);
    Mix_FreeChunk(test_wav);
}

TEST_F(audio_manager_test, test_play_ogg_full_volume) {
    auto test_ogg = Mix_LoadMUS("test_music.ogg");
    ASSERT_TRUE(test_ogg);
    ska::bytell_hash_map<uint16_t, Mix_Music *> assets;
    assets[1] = test_ogg;
    set_music(&assets);
    play_music(1, MIX_MAX_VOLUME, 3);
    SDL_Delay(10000);
    Mix_FreeMusic(test_ogg);
}

TEST_F(audio_manager_test, test_play_ogg_half_volume) {
    auto test_ogg = Mix_LoadMUS("test_music.ogg");
    ASSERT_TRUE(test_ogg);
    ska::bytell_hash_map<uint16_t, Mix_Music *> assets;
    assets[1] = test_ogg;
    set_music(&assets);
    play_music(1, MIX_MAX_VOLUME / 2, 3);
    SDL_Delay(10000);
    Mix_FreeMusic(test_ogg);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}