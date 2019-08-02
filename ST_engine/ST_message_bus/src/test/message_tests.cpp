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

TEST(message_test, test_message_creation) {
    auto test_subject = new message(1, make_data<bool>(true));
    ASSERT_TRUE(test_subject);
}

TEST(message_test, test_message_get_data) {
    auto test_subject = new message(1, make_data<bool>(true));
    ASSERT_TRUE(*static_cast<bool*>(test_subject->get_data()));
}

TEST(message_test, test_message_get_base_data) {
    auto test_subject = new message(1, 30, nullptr);
    ASSERT_FALSE(static_cast<bool>(test_subject->get_data()));
    ASSERT_EQ(30, test_subject->base_data0);
}

TEST(message_test, test_message_get_data_and_base_data) {
    auto test_subject = new message(1, 30, make_data<bool>(true));
    ASSERT_TRUE(*static_cast<bool*>(test_subject->get_data()));
    ASSERT_EQ(30, test_subject->base_data0);
}

TEST(message_test, test_message_copy) {
    auto test_subject = new message(1, make_data<int>(12));
    auto copy = test_subject->make_copy();
    ASSERT_TRUE(test_subject->get_data());
    ASSERT_TRUE(copy->get_data());
    ASSERT_EQ(test_subject->get_data(), copy->get_data());
    ASSERT_EQ(test_subject->msg_name, copy->msg_name);
    delete(test_subject);
    delete(copy);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}