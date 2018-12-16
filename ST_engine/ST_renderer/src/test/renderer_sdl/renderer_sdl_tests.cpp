/*This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */


#include <gtest/gtest.h>

#include <ST_util/test_util.hpp>
#include <renderer_sdl.hpp>

/// Tests fixture for the renderer_sdl
// These these are purely visual - you have to watch them run and
// look out for anything wrong. This means that you won't get
// a test failiure if for some reason nothing is rendered.
// However these are still useful as they provide you with a quick
// way to test if any of the specific rendering functions is broken.
class renderer_sdl_tests : public ::testing::Test {

protected:

    //Adjust testing conditions here
    uint32_t wait_duration = 1500;
    uint32_t fullscreen = 1;

    SDL_Window* test_window{};
    int16_t test_window_width = 0;
    int16_t test_window_height = 0;

    void SetUp() override{
        initialize_SDL();
        SDL_DisplayMode DM{};
        SDL_GetDisplayMode(0, 0, &DM);
        test_window_width = static_cast<int16_t>(DM.w);
        test_window_height = static_cast<int16_t>(DM.h);

        test_window = SDL_CreateWindow("TEST", 0, 0, test_window_width, test_window_height, 0);

        ST::renderer_sdl::initialize(test_window, test_window_width, test_window_height);

        SDL_SetWindowFullscreen(test_window, fullscreen);
        SDL_ShowCursor(0);

        ST::renderer_sdl::clear_screen();
    }

    void TearDown() override{
        ST::renderer_sdl::close();
        SDL_DestroyWindow(test_window);
        close_SDL();
    }
};

TEST_F(renderer_sdl_tests, test_set_draw_color){
    int counter = 0;
    for(uint32_t i = 0; i < wait_duration/180; i++) {
        if(counter == 0) {
            ST::renderer_sdl::set_draw_color(255, 0, 0, 255);
        }else if(counter == 1){
            ST::renderer_sdl::set_draw_color(0, 255, 0, 255);
        }else if(counter == 2){
            ST::renderer_sdl::set_draw_color(0, 0, 255, 255);
        }else if(counter == 3){
            ST::renderer_sdl::set_draw_color(255, 0, 255, 255);
        }else if(counter == 4){
            ST::renderer_sdl::set_draw_color(0, 255, 255, 255);
        }else if(counter == 5){
            ST::renderer_sdl::set_draw_color(255, 255, 0, 255);
        }else if(counter == 6){
            ST::renderer_sdl::set_draw_color(255, 100, 20, 255);
        }else{
            counter = 0;
        }
        counter++;
        ST::renderer_sdl::clear_screen();
        ST::renderer_sdl::present();
        SDL_Delay(160);
    }
}

TEST_F(renderer_sdl_tests, test_draw_rectangle_white){
    ST::renderer_sdl::draw_rectangle(100, 100, 300, 300, {255, 255, 255, 255});
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_filled_rectangle_white){
    ST::renderer_sdl::draw_rectangle_filled(100, 100, 300, 300, {255, 255, 255, 255});
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_filled_rectangle_white_transparent){
    ST::renderer_sdl::draw_rectangle_filled(100, 100, 300, 300, {255, 255, 255, 50});
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_filled_rectangle_red){
    ST::renderer_sdl::draw_rectangle_filled(100, 100, 300, 300, {255, 0, 0, 255});
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_filled_rectangle_green){
    ST::renderer_sdl::draw_rectangle_filled(100, 100, 300, 300, {0, 255, 0, 255});
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_filled_rectangle_blue){
    ST::renderer_sdl::draw_rectangle_filled(100, 100, 300, 300, {0, 0, 255, 255});
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_background){
    SDL_Surface* test_surface = IMG_Load("test_image_2.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    ST::renderer_sdl::draw_background(1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_texture){
    SDL_Surface* test_surface = IMG_Load("test_image_1.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    ST::renderer_sdl::draw_texture(1, 300, 300);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_texture_scaled){
    SDL_Surface* test_surface = IMG_Load("test_image_1.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    ST::renderer_sdl::draw_texture(1, 300, 300);
    ST::renderer_sdl::draw_texture_scaled(1, 800, 300, 0.5, 0.5);
    ST::renderer_sdl::draw_texture_scaled(1, 300, 700, 2, 2);
    ST::renderer_sdl::draw_texture_scaled(1, 800, 700, 2, 1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_font_english_small){
    int font_size = 20;
    TTF_Font* test_font = TTF_OpenFont("test_font.ttf", font_size);
    ASSERT_TRUE(static_cast<bool>(test_font));
    ska::bytell_hash_map<std::string, TTF_Font*> test_assets;
    test_assets["test_font.ttf" + std::to_string(font_size)] = test_font;
    ST::renderer_sdl::upload_fonts(&test_assets);
    ST::renderer_sdl::draw_text("test_font.ttf", "The quick brown fox!", 300, 300, {255, 0, 0, 255}, font_size, -1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_font_english_medium){
    int font_size = 50;
    TTF_Font* test_font = TTF_OpenFont("test_font.ttf", font_size);
    ASSERT_TRUE(static_cast<bool>(test_font));
    ska::bytell_hash_map<std::string, TTF_Font*> test_assets;
    test_assets["test_font.ttf" + std::to_string(font_size)] = test_font;
    ST::renderer_sdl::upload_fonts(&test_assets);
    ST::renderer_sdl::draw_text("test_font.ttf", "The quick brown fox!", 200, 300, {0, 255, 0, 255}, font_size, -1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_font_english_large){
    int font_size = 90;
    TTF_Font* test_font = TTF_OpenFont("test_font.ttf", font_size);
    ASSERT_TRUE(static_cast<bool>(test_font));
    ska::bytell_hash_map<std::string, TTF_Font*> test_assets;
    test_assets["test_font.ttf" + std::to_string(font_size)] = test_font;
    ST::renderer_sdl::upload_fonts(&test_assets);
    ST::renderer_sdl::draw_text("test_font.ttf", "The quick brown fox!", 100, 300, {0, 0, 255, 255}, font_size, -1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_font_russian_small){
    int font_size = 20;
    TTF_Font* test_font = TTF_OpenFont("test_font.ttf", font_size);
    ASSERT_TRUE(static_cast<bool>(test_font));
    ska::bytell_hash_map<std::string, TTF_Font*> test_assets;
    test_assets["test_font.ttf" + std::to_string(font_size)] = test_font;
    ST::renderer_sdl::upload_fonts(&test_assets);
    ST::renderer_sdl::draw_text("test_font.ttf", "Этот тест тестирует шрифты!", 300, 300, {255, 0, 0, 255}, font_size, -1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_font_russian_medium){
    int font_size = 50;
    TTF_Font* test_font = TTF_OpenFont("test_font.ttf", font_size);
    ASSERT_TRUE(static_cast<bool>(test_font));
    ska::bytell_hash_map<std::string, TTF_Font*> test_assets;
    test_assets["test_font.ttf" + std::to_string(font_size)] = test_font;
    ST::renderer_sdl::upload_fonts(&test_assets);
    ST::renderer_sdl::draw_text("test_font.ttf", "Этот тест тестирует шрифты!", 200, 300, {0, 255, 0, 255}, font_size, -1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_font_russian_large){
    int font_size = 90;
    TTF_Font* test_font = TTF_OpenFont("test_font.ttf", font_size);
    ASSERT_TRUE(static_cast<bool>(test_font));
    ska::bytell_hash_map<std::string, TTF_Font*> test_assets;
    test_assets["test_font.ttf" + std::to_string(font_size)] = test_font;
    ST::renderer_sdl::upload_fonts(&test_assets);
    ST::renderer_sdl::draw_text("test_font.ttf", "Этот тест тестирует шрифты!", 100, 300, {0, 0, 255, 255}, font_size, -1);
    ST::renderer_sdl::present();
    SDL_Delay(wait_duration);
}

TEST_F(renderer_sdl_tests, test_draw_sprite_animated1){
    SDL_Surface* test_surface = IMG_Load("test_sprite.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    for(uint32_t i = 0; i < wait_duration/16; i++) {
        uint32_t time = SDL_GetTicks() >> 7U;
        ST::renderer_sdl::clear_screen();
        ST::renderer_sdl::draw_sprite(1, 300, 500, time % 6, 1, 6, 6);
        ST::renderer_sdl::present();
        SDL_Delay(16);
    }
}

TEST_F(renderer_sdl_tests, test_draw_sprite_animated2){
    SDL_Surface* test_surface = IMG_Load("test_sprite.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    for(uint32_t i = 0; i < wait_duration/16; i++) {
        uint32_t time = SDL_GetTicks() >> 7U;
        ST::renderer_sdl::clear_screen();
        ST::renderer_sdl::draw_sprite(1, 300, 500, time % 6, 3, 6, 6);
        ST::renderer_sdl::present();
        SDL_Delay(16);
    }
}

TEST_F(renderer_sdl_tests, test_draw_sprite_scaled){
    SDL_Surface* test_surface = IMG_Load("test_sprite.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    for(uint32_t i = 0; i < wait_duration/16; i++) {
        uint32_t time = SDL_GetTicks() >> 7U;
        ST::renderer_sdl::clear_screen();
        ST::renderer_sdl::draw_sprite(1, 300, 500, time % 6, 1, 6, 6);
        ST::renderer_sdl::draw_sprite_scaled(1, 700, 500, time % 6, 1, 6, 6, 0.5, 0.5);
        ST::renderer_sdl::draw_sprite_scaled(1, 1000, 800, time % 6, 1, 6, 6, 2, 2);
        ST::renderer_sdl::present();
        SDL_Delay(16);
    }
}

TEST_F(renderer_sdl_tests, test_draw_overlay){
    SDL_Surface* test_surface = IMG_Load("test_overlay.png");
    ASSERT_TRUE(static_cast<bool>(test_surface));
    ska::bytell_hash_map<size_t, SDL_Surface*> test_assets;
    test_assets[1] = test_surface;
    ST::renderer_sdl::upload_surfaces(&test_assets);
    for(uint32_t i = 0; i < wait_duration/16; i++) {
        uint32_t time = SDL_GetTicks() >> 7U;
        ST::renderer_sdl::clear_screen();
        ST::renderer_sdl::draw_overlay(1, time % 6, 17);
        ST::renderer_sdl::present();
        SDL_Delay(16);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
