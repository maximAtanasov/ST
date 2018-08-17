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
#include <ST_util/test_util.hpp>

class message_allocator_tests : public::testing::Test {

protected:

    void SetUp() override{
        initialize_SDL();
    }

    void TearDown() override{
        close_SDL();
    }
};

TEST_F(message_allocator_tests, test_message_allocator) {
    message_allocator test_subject;
    message* test_message = test_subject.allocate_message(SET_FULLSCREEN, make_data(true));
    ASSERT_TRUE(test_message);
}

TEST_F(message_allocator_tests, allocate_max_fail) {
    message_allocator test_subject;
    for(uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++){
        ASSERT_TRUE(test_subject.allocate_message(SET_FULLSCREEN, make_data(true)));
    }
    ASSERT_FALSE(test_subject.allocate_message(SET_FULLSCREEN, make_data(true)));
}

TEST_F(message_allocator_tests, allocate_max_twice_after_deallocating_and_fail) {
    message_allocator test_subject;
    for(uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++){
        ASSERT_TRUE(test_subject.allocate_message(SET_FULLSCREEN, make_data(true)));
    }
    for(uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++){
        test_subject.deallocate(i);
    }
    for(uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++){
        ASSERT_TRUE(test_subject.allocate_message(SET_FULLSCREEN, make_data(true)));
    }
    ASSERT_FALSE(test_subject.allocate_message(SET_FULLSCREEN, make_data(true)));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}