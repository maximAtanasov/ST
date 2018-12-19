/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <window_manager.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the display_manager
class window_manager_tests : public ::testing::Test {

protected:

    void set_fullscreen(window_manager* test_mngr){
        test_mngr->set_fullscreen(true);
    }

    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(window_manager_tests, set_fullscreen_test){
    window_manager test_mngr(new message_bus(), nullptr, " ");
    set_fullscreen(&test_mngr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}