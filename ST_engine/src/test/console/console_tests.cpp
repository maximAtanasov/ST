/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <message_bus/message_bus.hpp>
#include <console/console.hpp>

/// Tests fixture for the console
class console_test : public ::testing::Test {

protected:
    console* test_cnsl{};

    uint8_t get_log_level(){
        return test_cnsl->log_level;
    }

    std::string pop_string(){
        std::string str = test_cnsl->entries.at(0).text;
        test_cnsl->entries.erase(test_cnsl->entries.begin());
        return str;
    }

    void write(const ST::console_log& log){
        test_cnsl->write(log);
    }

    void SetUp() override{
        test_cnsl = new console(new message_bus());
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
    write(ST::console_log(ST::log_type::ERROR, "TEST_STRING"));
    EXPECT_EQ("TEST_STRING", pop_string());
}

TEST_F(console_test, console_write_info) {
    test_cnsl->set_log_level(ST::log_type::INFO);
    write(ST::console_log(ST::console_log(ST::log_type::INFO, "TEST_STRING")));
    EXPECT_EQ("TEST_STRING", pop_string());
}


TEST_F(console_test, console_write_all) {
    test_cnsl->set_log_level(ST::log_type::SUCCESS | ST::log_type::INFO | ST::log_type::ERROR);

    write(ST::console_log(ST::console_log(ST::log_type::INFO, "TEST_STRING")));
    write(ST::console_log(ST::console_log(ST::log_type::ERROR, "TEST_STRING2")));
    write(ST::console_log(ST::console_log(ST::log_type::INFO, "TEST_STRING3")));

    EXPECT_EQ("TEST_STRING", pop_string());
    EXPECT_EQ("TEST_STRING2", pop_string());
    EXPECT_EQ("TEST_STRING3", pop_string());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}