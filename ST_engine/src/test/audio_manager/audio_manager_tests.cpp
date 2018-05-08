/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <audio_manager/audio_manager.hpp>
#include <ST_util/test_util.hpp>

/// Tests fixture for the audio_manager
class audio_manager_test : public ::testing::Test {

protected:

    int get_volume(){
        return test_mngr.volume;
    }

    void update_task(void* arg){
        audio_manager::update_task(arg);
    }

    message_bus msg_bus{};
    audio_manager test_mngr;

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

TEST_F(audio_manager_test, set_volume) {
    auto temp_sub = new subscriber();
    msg_bus.subscribe(VOLUME_LEVEL, temp_sub);
    msg_bus.send_msg(make_msg(SET_VOLUME, make_data<int>(50)));
    update_task(&test_mngr);
    EXPECT_EQ(50, get_volume());

    message* temp = temp_sub->get_next_message();
    ASSERT_EQ(true, static_cast<bool>(temp));
    ASSERT_EQ(50, *static_cast<int*>(temp->get_data()));
}

TEST_F(audio_manager_test, toggle_audio) {
    auto temp_sub = new subscriber();
    msg_bus.subscribe(VOLUME_LEVEL, temp_sub);

    msg_bus.send_msg(make_msg(TOGGLE_AUDIO, nullptr));
    update_task(&test_mngr);
    ASSERT_EQ(0, get_volume());

    message* temp = temp_sub->get_next_message();
    ASSERT_EQ(true, static_cast<bool>(temp));
    ASSERT_EQ(0, *static_cast<int*>(temp->get_data()));

    msg_bus.send_msg(make_msg(TOGGLE_AUDIO, nullptr));
    update_task(&test_mngr);
    ASSERT_EQ(MIX_MAX_VOLUME, get_volume());

    temp = temp_sub->get_next_message();
    ASSERT_EQ(true, static_cast<bool>(temp));
    ASSERT_EQ(MIX_MAX_VOLUME, *static_cast<int*>(temp->get_data()));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}