/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <ST_loaders/loaders.hpp>
#include <ST_util/test_util.hpp>
#include <fstream>

#ifdef _MSC_VER
#define chdir _chdir
#endif

TEST(loaders_tests, test_get_file_extension){
    std::string test_wav = "sound.wav";
    std::string test_mp3 = "music.mp3";
    std::string test_ogg = "music.ogg";
    std::string test_png = "image.png";
    std::string test_webp = "image.webp";
    std::string test_bin = "image.bin";

    EXPECT_EQ("wav", ST::get_file_extension(test_wav));
    EXPECT_EQ("mp3", ST::get_file_extension(test_mp3));
    EXPECT_EQ("ogg", ST::get_file_extension(test_ogg));
    EXPECT_EQ("png", ST::get_file_extension(test_png));
    EXPECT_EQ("webp", ST::get_file_extension(test_webp));
    EXPECT_EQ("bin", ST::get_file_extension(test_bin));
    EXPECT_EQ("unknown", ST::get_file_extension("no_extension"));
 }

TEST(loaders_tests, test_get_file_extension_unknown){
    std::string test_wav = "soundwav";
    std::string test_mp3 = "musicmp3";
    std::string test_ogg = "musicogg";
    std::string test_png = "imagepng";
    std::string test_webp = "imagewebp";
    std::string test_bin = "testbin";


    EXPECT_EQ("unknown", ST::get_file_extension(test_wav));
    EXPECT_EQ("unknown", ST::get_file_extension(test_mp3));
    EXPECT_EQ("unknown", ST::get_file_extension(test_ogg));
    EXPECT_EQ("unknown", ST::get_file_extension(test_png));
    EXPECT_EQ("unknown", ST::get_file_extension(test_webp));
    EXPECT_EQ("unknown", ST::get_file_extension(test_bin));
    EXPECT_EQ("unknown", ST::get_file_extension("no_extension"));
}

TEST(loaders_tests, test_pack_to_binary){

    //Set up
    initialize_SDL();
    std::vector<std::string> filenames = {"test_image_1.png", "test_image_3.webp", "test_sound_1.wav", "test_music_1.ogg"};
    std::string binary_name = "result_binary";
    ST::pack_to_binary(binary_name, filenames);

    long expected_size = get_file_size(filenames.at(0)) + get_file_size(filenames.at(1))
            + get_file_size(filenames.at(2)) + get_file_size(filenames.at(3));
    long binary_size = get_file_size(binary_name);

    //Tear Down
    remove(binary_name.c_str());
    close_SDL();

    ASSERT_NEAR(expected_size, binary_size, 200);
}

TEST(loaders_tests, test_unpack_binary_to_disk){
    //Set up
    initialize_SDL();
    ASSERT_EQ(0, chdir("unpack_test/"));
    std::string binary_name = "test_binary_complex.bin";

    //Test
    ASSERT_EQ(0, ST::unpack_binary_to_disk(binary_name));
    ASSERT_TRUE(Mix_LoadMUS("test_music_1.ogg"));
    ASSERT_TRUE(Mix_LoadMUS("test_music_2.ogg"));
    ASSERT_TRUE(Mix_LoadWAV("test_sound_1.wav"));
    ASSERT_TRUE(Mix_LoadWAV("test_sound_2.wav"));
    ASSERT_TRUE(IMG_Load("test_image_1.png"));
    ASSERT_TRUE(IMG_Load("test_image_2.png"));
    ASSERT_TRUE(IMG_Load("test_image_3.webp"));

    //Tear Down
    remove("test_image_1.png");
    remove("test_image_2.png");
    remove("test_image_3.webp");
    remove("test_music_1.ogg");
    remove("test_music_2.ogg");
    remove("test_sound_1.wav");
    remove("test_sound_2.wav");
    close_SDL();
    ASSERT_EQ(0, chdir("../"));
}


TEST(loaders_tests, test_unpack_binary){
    //Set up
    initialize_SDL();
    ASSERT_EQ(0, chdir("unpack_test/"));
    std::string binary_name = "test_binary_complex.bin";

    //Test
    ST::assets_named* result = ST::unpack_binary(binary_name);
    ASSERT_TRUE(result);
    ASSERT_EQ(2, result->music.size());
    ASSERT_EQ(2, result->chunks.size());
    ASSERT_EQ(3, result->surfaces.size());

    ASSERT_TRUE(result->music.at("test_music_1.ogg"));
    ASSERT_TRUE(result->music.at("test_music_2.ogg"));
    ASSERT_TRUE(result->chunks.at("test_sound_1.wav"));
    ASSERT_TRUE(result->chunks.at("test_sound_2.wav"));
    ASSERT_TRUE(result->surfaces.at("test_image_1.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_2.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_3.webp"));

    //Tear down
    Mix_FreeMusic(result->music.at("test_music_1.ogg"));
    Mix_FreeMusic(result->music.at("test_music_2.ogg"));
    Mix_FreeChunk(result->chunks.at("test_sound_1.wav"));
    Mix_FreeChunk(result->chunks.at("test_sound_2.wav"));
    SDL_FreeSurface(result->surfaces.at("test_image_1.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_2.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_3.webp"));
    close_SDL();
    ASSERT_EQ(0, chdir("../"));
}

TEST(loaders_tests, test_ignore_identical_file_when_packing_to_binary){

    //Set up
    initialize_SDL();
    std::vector<std::string> filenames = {"test_image_1.png", "test_image_1.png", "test_sound_1.wav", "test_music_1.ogg"};
    std::string binary_name = "result_binary";
    ST::pack_to_binary(binary_name, filenames);

    long expected_size = get_file_size(filenames.at(0)) + get_file_size(filenames.at(2)) + get_file_size(filenames.at(3));
    long binary_size = get_file_size(binary_name);

    //Tear Down
    remove(binary_name.c_str());
    close_SDL();

    ASSERT_NEAR(expected_size, binary_size, 200);
}

TEST(loaders_tests, test_can_add_to_existing_binary){

    //Set up========================================

    initialize_SDL();
    ASSERT_EQ(0, chdir("unpack_test/"));
    std::vector<std::string> filenames = {"test_image_4.png", "test_image_5.png"};

    std::string input_binary_name = "test_binary_complex.bin";
    std::string result_binary_name = "test_binary_complex_copy.bin";

    //Create a copy of the test binary
    std::ifstream  src(input_binary_name, std::ios::binary);
    std::ofstream  dst(result_binary_name,   std::ios::binary);
    dst << src.rdbuf();

    //Test==========================================

    ST::add_to_binary(result_binary_name, filenames);

    //Check that the filesize is about right
    long expected_size = get_file_size(filenames.at(0)) + get_file_size(filenames.at(1))
                         + get_file_size(input_binary_name);

    long binary_size = get_file_size(result_binary_name);

    ASSERT_NEAR(expected_size, binary_size, 200);

    //Check that everything is still there + the new assets
    ST::assets_named* result = ST::unpack_binary(result_binary_name);
    ASSERT_TRUE(result);
    ASSERT_EQ(2, result->music.size());
    ASSERT_EQ(2, result->chunks.size());
    ASSERT_EQ(5, result->surfaces.size());

    ASSERT_TRUE(result->music.at("test_music_1.ogg"));
    ASSERT_TRUE(result->music.at("test_music_2.ogg"));
    ASSERT_TRUE(result->chunks.at("test_sound_1.wav"));
    ASSERT_TRUE(result->chunks.at("test_sound_2.wav"));
    ASSERT_TRUE(result->surfaces.at("test_image_1.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_2.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_3.webp"));
    ASSERT_TRUE(result->surfaces.at("test_image_4.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_5.png"));

    //Tear down ===============================================
    Mix_FreeMusic(result->music.at("test_music_1.ogg"));
    Mix_FreeMusic(result->music.at("test_music_2.ogg"));
    Mix_FreeChunk(result->chunks.at("test_sound_1.wav"));
    Mix_FreeChunk(result->chunks.at("test_sound_2.wav"));
    SDL_FreeSurface(result->surfaces.at("test_image_1.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_2.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_3.webp"));
    SDL_FreeSurface(result->surfaces.at("test_image_4.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_5.png"));

    remove(result_binary_name.c_str());
    close_SDL();
    ASSERT_EQ(0, chdir("../"));
}

TEST(loaders_tests, test_exit_when_duplicate_name_found_in_exisitng_library) {

    //Set up========================================

    initialize_SDL();
    ASSERT_EQ(0, chdir("unpack_test/"));
    std::vector<std::string> filenames = {"test_image_2.png", "test_image_5.png"};

    std::string input_binary_name = "test_binary_complex.bin";
    std::string result_binary_name = "test_binary_complex_copy.bin";

    //Create a copy of the test binary
    std::ifstream  src(input_binary_name, std::ios::binary);
    std::ofstream  dst(result_binary_name,   std::ios::binary);
    dst << src.rdbuf();

    //Test==========================================

    ASSERT_EQ(-2, ST::add_to_binary(result_binary_name, filenames));

    //Check that nothing was modified
    ST::assets_named* result = ST::unpack_binary(result_binary_name);
    ASSERT_TRUE(result);
    ASSERT_EQ(2, result->music.size());
    ASSERT_EQ(2, result->chunks.size());
    ASSERT_EQ(3, result->surfaces.size());

    ASSERT_TRUE(result->music.at("test_music_1.ogg"));
    ASSERT_TRUE(result->music.at("test_music_2.ogg"));
    ASSERT_TRUE(result->chunks.at("test_sound_1.wav"));
    ASSERT_TRUE(result->chunks.at("test_sound_2.wav"));
    ASSERT_TRUE(result->surfaces.at("test_image_1.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_2.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_3.webp"));

    //Tear down ===============================================
    Mix_FreeMusic(result->music.at("test_music_1.ogg"));
    Mix_FreeMusic(result->music.at("test_music_2.ogg"));
    Mix_FreeChunk(result->chunks.at("test_sound_1.wav"));
    Mix_FreeChunk(result->chunks.at("test_sound_2.wav"));
    SDL_FreeSurface(result->surfaces.at("test_image_1.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_2.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_3.webp"));

    close_SDL();
    remove(result_binary_name.c_str());
    ASSERT_EQ(0, chdir("../"));
}

TEST(loaders_tests, test_exit_when_duplicate_name_in_different_directory_found_in_exisitng_library) {

    //Set up========================================

    initialize_SDL();
    ASSERT_EQ(0, chdir("unpack_test/"));
    std::vector<std::string> filenames = {"../test_image_2.png", "test_image_5.png"};

    std::string input_binary_name = "test_binary_complex.bin";
    std::string result_binary_name = "test_binary_complex_copy.bin";

    //Create a copy of the test binary
    std::ifstream  src(input_binary_name, std::ios::binary);
    std::ofstream  dst(result_binary_name,   std::ios::binary);
    dst << src.rdbuf();

    //Test==========================================

    ASSERT_EQ(-2, ST::add_to_binary(result_binary_name, filenames));

    //Check that nothing was modified
    ST::assets_named* result = ST::unpack_binary(result_binary_name);
    ASSERT_TRUE(result);
    ASSERT_EQ(2, result->music.size());
    ASSERT_EQ(2, result->chunks.size());
    ASSERT_EQ(3, result->surfaces.size());

    ASSERT_TRUE(result->music.at("test_music_1.ogg"));
    ASSERT_TRUE(result->music.at("test_music_2.ogg"));
    ASSERT_TRUE(result->chunks.at("test_sound_1.wav"));
    ASSERT_TRUE(result->chunks.at("test_sound_2.wav"));
    ASSERT_TRUE(result->surfaces.at("test_image_1.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_2.png"));
    ASSERT_TRUE(result->surfaces.at("test_image_3.webp"));

    //Tear down ===============================================
    Mix_FreeMusic(result->music.at("test_music_1.ogg"));
    Mix_FreeMusic(result->music.at("test_music_2.ogg"));
    Mix_FreeChunk(result->chunks.at("test_sound_1.wav"));
    Mix_FreeChunk(result->chunks.at("test_sound_2.wav"));
    SDL_FreeSurface(result->surfaces.at("test_image_1.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_2.png"));
    SDL_FreeSurface(result->surfaces.at("test_image_3.webp"));

    close_SDL();
    remove(result_binary_name.c_str());
    ASSERT_EQ(0, chdir("../"));
}

TEST(loaders_tests, test_pack_to_binary_calls_add_to_binary_on_existing_file) {

    //Set up========================================

    initialize_SDL();
    ASSERT_EQ(0, chdir("unpack_test/"));
    std::vector<std::string> filenames = {"../test_image_2.png", "test_image_5.png"};

    std::string input_binary_name = "test_binary_complex.bin";
    std::string result_binary_name = "test_binary_complex_copy.bin";

    //Create a copy of the test binary
    std::ifstream  src(input_binary_name, std::ios::binary);
    std::ofstream  dst(result_binary_name,   std::ios::binary);
    dst << src.rdbuf();

    //Test==========================================

    //We test this by checking for clashing names with the existing binary.
    //If add_to_binary was called, -2 should be returned.
    ASSERT_EQ(-2, ST::pack_to_binary(result_binary_name, filenames));

    //Tear down
    close_SDL();
    remove(result_binary_name.c_str());
    ASSERT_EQ(0, chdir("../"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}