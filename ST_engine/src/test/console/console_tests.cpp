#include <gtest/gtest.h>
#include <message_bus/message_bus.hpp>
#include <console/console.hpp>

/// Tests fixture for the console
class console_test : public ::testing::Test {

protected:
    message_bus msg_bus{};
    console* test_cnsl;

    uint8_t get_log_level(){
        return test_cnsl->log_level;
    }

    std::string pop_string(){
        std::string str = test_cnsl->entries.at(0).text;
        test_cnsl->entries.erase(test_cnsl->entries.begin());
        return str;
    }

    void SetUp() override{
        test_cnsl = new console(&msg_bus);
    }

    void TearDown() override{
        delete test_cnsl;
    }
};

TEST_F(console_test, set_log_level) {
    test_cnsl->set_log_level(ST::log_type::ERROR);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::ERROR), get_log_level());

    test_cnsl->set_log_level(ST::log_type::INFO);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::INFO), get_log_level());

    test_cnsl->set_log_level(ST::log_type::SUCCESS);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::SUCCESS), get_log_level());

    test_cnsl->set_log_level(ST::log_type::SUCCESS | ST::log_type::ERROR);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::SUCCESS | ST::log_type::ERROR), get_log_level());

    test_cnsl->set_log_level(ST::log_type::INFO | ST::log_type::ERROR);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::INFO | ST::log_type::ERROR), get_log_level());

    test_cnsl->set_log_level(ST::log_type::INFO | ST::log_type::SUCCESS);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::INFO | ST::log_type::SUCCESS), get_log_level());

    test_cnsl->set_log_level(ST::log_type::INFO | ST::log_type::SUCCESS | ST::log_type::ERROR);
    EXPECT_EQ(static_cast<uint8_t>(ST::log_type::INFO | ST::log_type::SUCCESS | ST::log_type::ERROR), get_log_level());
}

TEST_F(console_test, console_write_error) {
    test_cnsl->set_log_level(ST::log_type::ERROR);
    msg_bus.send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(ST::log_type::ERROR, "TEST_STRING"))));
    test_cnsl->update();
    EXPECT_EQ("TEST_STRING", pop_string());
}

TEST_F(console_test, console_write_info) {
    test_cnsl->set_log_level(ST::log_type::INFO);
    msg_bus.send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(ST::log_type::INFO, "TEST_STRING"))));
    test_cnsl->update();
    EXPECT_EQ("TEST_STRING", pop_string());
}


TEST_F(console_test, console_write_all) {
    test_cnsl->set_log_level(ST::log_type::SUCCESS | ST::log_type::INFO | ST::log_type::ERROR);
    msg_bus.send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(ST::log_type::SUCCESS, "TEST_STRING"))));
    msg_bus.send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(ST::log_type::ERROR, "TEST_STRING2"))));
    msg_bus.send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(ST::log_type::INFO, "TEST_STRING3"))));
    test_cnsl->update();
    EXPECT_EQ("TEST_STRING", pop_string());
    EXPECT_EQ("TEST_STRING2", pop_string());
    EXPECT_EQ("TEST_STRING3", pop_string());
}

TEST_F(console_test, console_toggle) {
    msg_bus.send_msg(make_msg(CONSOLE_TOGGLE, make_data<ST::console_log>(ST::console_log(ST::log_type::INFO, "TEST_STRING"))));
    test_cnsl->update();
    EXPECT_EQ(true, test_cnsl->is_open());

    msg_bus.send_msg(make_msg(CONSOLE_TOGGLE, make_data<ST::console_log>(ST::console_log(ST::log_type::INFO, "TEST_STRING"))));
    test_cnsl->update();
    EXPECT_EQ(false, test_cnsl->is_open());
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}