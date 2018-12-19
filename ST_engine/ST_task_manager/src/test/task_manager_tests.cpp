/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <gtest/gtest.h>
#include <task_manager.hpp>

class task_manager_tests : public::testing::TestWithParam<int>{

protected:

    message_bus* msg_bus{};

    void SetUp() override{
        msg_bus = new message_bus();
    }

    void TearDown() override{
        delete msg_bus;
    }
};

static void test_task_function(void* arg){
    auto val = static_cast<uint8_t*>(arg);
    ++*val;
}

static void test_task_function2(void* arg){
    auto val = static_cast<uint8_t*>(arg);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ++*val;
}

TEST_F(task_manager_tests, test_start_task_without_dependency){
    //Set up
    task_manager test_subject(msg_bus);
    uint8_t test_value = 10;

    //Test
    task_id id = test_subject.start_task(make_task(test_task_function, &test_value, nullptr));
    test_subject.wait_for_task(id);
    ASSERT_EQ(test_value, 11);
}

TEST_F(task_manager_tests, test_start_tasks_with_dependency){
    //Set up
    task_manager test_subject(msg_bus);
    uint8_t test_value = 10;

    //Test
    task_id id1 = test_subject.start_task(make_task(test_task_function2, &test_value, nullptr));
    task_id id2 = test_subject.start_task(make_task(test_task_function, &test_value, id1));

    test_subject.wait_for_task(id2);
    ASSERT_EQ(test_value, 12);
}

TEST_F(task_manager_tests, test_start_task_lockfree_without_dependency){
    //Set up
    task_manager test_subject(msg_bus);
    uint8_t test_value = 10;

    //Test
    test_subject.start_task_lockfree(make_task(test_task_function, &test_value, nullptr));

    //Wait a suffecent amount of time
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(test_value, 11);
}

TEST_F(task_manager_tests, test_start_task_lockfree_with_dependency){
    //Set up
    task_manager test_subject(msg_bus);
    uint8_t test_value = 10;

    //Test
    task_id id1 = test_subject.start_task(make_task(test_task_function2, &test_value, nullptr));
    test_subject.start_task_lockfree(make_task(test_task_function, &test_value, id1));

    std::this_thread::sleep_for(std::chrono::seconds(4));
    ASSERT_EQ(test_value, 12);
}

TEST_P(task_manager_tests, test_do_work_while_waiting){
    //Set up
    task_manager test_subject(msg_bus);
    //test_subject.set_task_thread_amount(4);
    uint8_t test_value1 = 10;
    uint8_t test_value2 = 20;
    //Test
    auto start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());


    auto semaphore_ = new semaphore();

    task_id id1 = test_subject.start_task(make_task(test_task_function2, &test_value1, nullptr));
    test_subject.start_task_lockfree(make_task(test_task_function, &test_value2, semaphore_));

    semaphore_->notify();
    test_subject.wait_for_task(id1);

    auto end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());


    ASSERT_NEAR(static_cast<double>(end.count()-start.count()), 2100, 200);
    ASSERT_EQ(test_value1, 11);
    ASSERT_EQ(test_value2, 21);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}