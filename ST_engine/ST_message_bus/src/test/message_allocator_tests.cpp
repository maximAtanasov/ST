/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <gtest/gtest.h>
#include "../../include/message_bus.hpp"

TEST(message_allocator_tests, test_message_allocator) {
    message_allocator test_subject;
    message* test_message = test_subject.allocate_message(1, make_data(true));
    ASSERT_TRUE(test_message);
}

TEST(message_allocator_tests, allocate_max_fail) {
    message_allocator test_subject;
    for(uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++){
        ASSERT_TRUE(test_subject.allocate_message(1, make_data(true)));
    }
    ASSERT_EXIT(test_subject.allocate_message(1, make_data(true)), ::testing::ExitedWithCode(1),
            "message bus: too many messages allocated, not enough memory, exiting");
}

TEST(message_allocator_tests, allocate_max_twice_after_deallocating_and_fail) {
    message_allocator test_subject;
    for (uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++) {
        ASSERT_TRUE(test_subject.allocate_message(1, make_data(true)));
    }
    for (uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++) {
        test_subject.deallocate(i);
    }
    for (uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++) {
        ASSERT_TRUE(test_subject.allocate_message(1, make_data(true)));
    }
    ASSERT_EXIT(test_subject.allocate_message(1, make_data(true)), ::testing::ExitedWithCode(1),
                "message bus: too many messages allocated, not enough memory, exiting");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}