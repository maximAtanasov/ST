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

TEST(message_allocator_tests, allocate_max) {
    message_allocator test_subject;
    for(uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++){
        ASSERT_TRUE(test_subject.allocate_message(1, make_data(true)));
    }
}

TEST(message_allocator_tests, allocate_max_twice_after_deallocating) {
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
}

TEST(message_allocator_tests, stress_test) {
    message_allocator test_subject;
    for(uint32_t j = 0; j < 300000; j++) {
        for (uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++) {
            ASSERT_TRUE(test_subject.allocate_message(1, make_data(true)));
        }
        for (uint16_t i = 0; i < MESSAGE_ALLOCATOR_CAPACITY; i++) {
            test_subject.deallocate(i);
        }
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}