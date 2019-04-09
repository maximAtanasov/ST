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
#include <message_bus.hpp>

class message_bus_tests : public::testing::Test {

protected:

    std::vector<subscriber*> get_subscribers(message_bus* test_subject, uint8_t msg){
        return test_subject->subscribers[msg];
    }
};

TEST_F(message_bus_tests, test_one_subscriber_to_message){
    //Set up
    uint8_t msg = 1;
    message_bus test_subject;
    subscriber test_subscriber;

    //Test
    test_subject.subscribe(msg, &test_subscriber);
    std::vector<subscriber*> subscribers_in_bus = get_subscribers(&test_subject, msg);
    ASSERT_EQ(1, subscribers_in_bus.size());
    ASSERT_EQ(&test_subscriber, subscribers_in_bus.at(0));
}

TEST_F(message_bus_tests, test_five_subscribers_to_message){
    //Set up
    uint8_t msg = 1;
    message_bus test_subject;
    subscriber test_subscriber1;
    subscriber test_subscriber2;
    subscriber test_subscriber3;
    subscriber test_subscriber4;
    subscriber test_subscriber5;

    //Test
    test_subject.subscribe(msg, &test_subscriber1);
    test_subject.subscribe(msg, &test_subscriber2);
    test_subject.subscribe(msg, &test_subscriber3);
    test_subject.subscribe(msg, &test_subscriber4);
    test_subject.subscribe(msg, &test_subscriber5);

    std::vector<subscriber*> subscribers_in_bus = get_subscribers(&test_subject, msg);

    ASSERT_EQ(5, subscribers_in_bus.size());
    ASSERT_EQ(&test_subscriber1, subscribers_in_bus.at(0));
    ASSERT_EQ(&test_subscriber2, subscribers_in_bus.at(1));
    ASSERT_EQ(&test_subscriber3, subscribers_in_bus.at(2));
    ASSERT_EQ(&test_subscriber4, subscribers_in_bus.at(3));
    ASSERT_EQ(&test_subscriber5, subscribers_in_bus.at(4));
}

TEST_F(message_bus_tests, test_one_subscriber_to_two_messages){
    //Set up
    uint8_t msg1 = 2;
    uint8_t msg2 = 3;
    message_bus test_subject;
    subscriber test_subscriber;

    //Test
    test_subject.subscribe(msg1, &test_subscriber);
    test_subject.subscribe(msg2, &test_subscriber);

    std::vector<subscriber*> subscribers_in_bus_for_msg1 = get_subscribers(&test_subject, msg1);
    std::vector<subscriber*> subscribers_in_bus_for_msg2 = get_subscribers(&test_subject, msg2);

    ASSERT_EQ(1, subscribers_in_bus_for_msg1.size());
    ASSERT_EQ(&test_subscriber, subscribers_in_bus_for_msg1.at(0));

    ASSERT_EQ(1, subscribers_in_bus_for_msg2.size());
    ASSERT_EQ(&test_subscriber, subscribers_in_bus_for_msg2.at(0));
}

TEST_F(message_bus_tests, test_send_message_to_one_subscriber){

    //Set up
    uint8_t msg = 1;
    message_bus test_subject;
    subscriber test_subscriber;
    test_subject.subscribe(msg, &test_subscriber);

    //Test
    test_subject.send_msg(new message(msg, make_data(20)));
    message* result = test_subscriber.get_next_message();
    ASSERT_TRUE(result);
    ASSERT_EQ(msg, result->msg_name);
    ASSERT_EQ(20, *static_cast<int*>(result->get_data()));
    delete(result);
}

TEST_F(message_bus_tests, test_send_message_to_two_subscribers){

    //Set up
    uint8_t msg = 2;
    message_bus test_subject;
    subscriber test_subscriber1;
    subscriber test_subscriber2;
    test_subject.subscribe(msg, &test_subscriber1);
    test_subject.subscribe(msg, &test_subscriber2);

    //Test
    test_subject.send_msg(new message(msg, make_data(20)));
    message* result1 = test_subscriber1.get_next_message();
    message* result2 = test_subscriber2.get_next_message();

    ASSERT_TRUE(result1);
    ASSERT_EQ(msg, result1->msg_name);
    ASSERT_EQ(20, *static_cast<int*>(result1->get_data()));
    delete(result1);

    ASSERT_TRUE(result2);
    ASSERT_EQ(msg, result2->msg_name);
    ASSERT_EQ(20, *static_cast<int*>(result2->get_data()));
    delete(result2);
}

TEST_F(message_bus_tests, test_send_two_messages_to_one_subscriber){

    //Set up
    uint8_t msg1 = 2;
    uint8_t msg2 = 3;
    message_bus test_subject;
    subscriber test_subscriber;
    test_subject.subscribe(msg1, &test_subscriber);
    test_subject.subscribe(msg2, &test_subscriber);
    //Test
    test_subject.send_msg(new message(msg1, make_data(20)));
    test_subject.send_msg(new message(msg2, make_data(30)));

    message* result1 = test_subscriber.get_next_message();
    message* result2 = test_subscriber.get_next_message();

    ASSERT_TRUE(result1);
    ASSERT_EQ(msg1, result1->msg_name);
    ASSERT_EQ(20, *static_cast<int*>(result1->get_data()));
    delete(result1);

    ASSERT_TRUE(result2);
    ASSERT_EQ(msg2, result2->msg_name);
    ASSERT_EQ(30, *static_cast<int*>(result2->get_data()));
    delete(result2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}