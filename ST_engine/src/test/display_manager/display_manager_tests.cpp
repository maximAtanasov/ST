/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <display_manager/display_manager.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the display_manager
class display_manager_tests : public ::testing::Test {

protected:

    void set_fullscreen(display_manager* test_mngr){
        test_mngr->set_fullscreen(true);
    }

    void SetUp() override{
        initialize_SDL();
    }

    void TearDown() override{
        close_SDL();
    }
};

TEST_F(display_manager_tests, set_fullscreen_test){
    display_manager test_mngr(new message_bus(), nullptr);
    set_fullscreen(&test_mngr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}