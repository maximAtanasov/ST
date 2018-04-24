/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>

#include <ST_util/test_util.hpp>
#include <display_manager/display_manager.hpp>

/// Tests fixture for the display_manager
class display_manager_tests : public ::testing::Test {

protected:


    void update_task(void* arg){
        display_manager::update_task(arg);
    }

    message_bus msg_bus{};
    display_manager test_mngr;

    void SetUp() override{
        initialize_SDL();
        msg_bus.initialize();
        test_mngr.initialize(&msg_bus, nullptr);
    }

    void TearDown() override{
        test_mngr.close();
        msg_bus.close();
        close_SDL();
    }
};

TEST_F(display_manager_tests, set_fullscreen){
    auto temp_sub = new subscriber();
    msg_bus.subscribe(FULLSCREEN_STATUS, temp_sub);
    msg_bus.send_msg(make_msg(SET_FULLSCREEN, make_data<bool>(true)));
    update_task(&test_mngr);
    message* temp = temp_sub->get_next_message();
    ASSERT_EQ(true, static_cast<bool>(temp));
    ASSERT_EQ(true, *static_cast<bool*>(temp->get_data()));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}