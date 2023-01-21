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

TEST(message_test, test_push_message_and_get_one_message) {
    auto test_subject = new subscriber();
    test_subject->push_message(new message(1, make_data(23)));
    auto message = test_subject->get_next_message();
    ASSERT_TRUE(message);
    ASSERT_FALSE(test_subject->get_next_message());
    ASSERT_EQ(23, *static_cast<int *>(message->get_data()));
    delete (message);
    delete (test_subject);
}

TEST(message_test, test_push_message_and_get_100_messages) {
    auto test_subject = new subscriber();
    for (uint16_t i = 0; i < 100; i++) {
        test_subject->push_message(new message(1, make_data(i)));
    }
    for (uint16_t i = 0; i < 100; i++) {
        auto message = test_subject->get_next_message();
        ASSERT_TRUE(message);
        ASSERT_EQ(i, *static_cast<uint16_t *>(message->get_data()));
        delete (message);
    }
    ASSERT_FALSE(test_subject->get_next_message());
    delete (test_subject);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}