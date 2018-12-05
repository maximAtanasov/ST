/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <game_manager/level/level.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the level object
class level_tests : public ::testing::Test {

protected:

    int8_t load_input_conf(ST::level* lvl){
        return lvl->load_input_conf();
    }

    void SetUp() override{
        initialize_SDL();
    }

    void TearDown() override{
        close_SDL();
    }
};

TEST_F(level_tests, test_load_input_configuration){

    //Set up
    ST::level level("test_level", new message_bus());
    std::hash<std::string> hash_f;

    std::string key1 = "INVENTORY";
    size_t key1_hash = hash_f(key1);

    std::string key2 = "GO";
    size_t key2_hash = hash_f(key2);

    std::string key3 = "JUMP";
    size_t key3_hash = hash_f(key3);

    //Test
    ASSERT_EQ(0, load_input_conf(&level));
    ska::bytell_hash_map<size_t, std::vector<ST::key>> actions_buttons = level.actions_Buttons;

    ST::key check_key1 = actions_buttons.at(key1_hash).at(0);
    ASSERT_EQ(ST::key::I, check_key1);

    ST::key check_key2 = actions_buttons.at(key1_hash).at(1);
    ASSERT_EQ(ST::key::CONTROLLER_BUTTON_B, check_key2);

    ST::key check_key3 = actions_buttons.at(key2_hash).at(0);
    ASSERT_EQ(ST::key::ENTER, check_key3);

    ST::key check_key4 = actions_buttons.at(key3_hash).at(0);
    ASSERT_EQ(ST::key::SPACEBAR, check_key4);

    ST::key check_key5 = actions_buttons.at(key3_hash).at(1);
    ASSERT_EQ(ST::key::CONTROLLER_BUTTON_A, check_key5);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}