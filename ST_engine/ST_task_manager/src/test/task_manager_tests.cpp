/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <gtest/gtest.h>
#include <task_manager.hpp>
#include <thread>

class task_manager_tests : public ::testing::TestWithParam<int> {
};

static void test_task_function(void *arg) {
    auto val = static_cast<uint8_t *>(arg);
    ++*val;
}

static void test_task_function2(void *arg) {
    auto val = static_cast<uint8_t *>(arg);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ++*val;
}

TEST_F(task_manager_tests, test_start_task_without_dependency) {
    //Set up
    task_manager test_subject;
    uint8_t test_value = 10;

    //Test
    task_id id = test_subject.start_task(new ST::task(test_task_function, &test_value, nullptr));
    test_subject.wait_for_task(id);
    ASSERT_EQ(test_value, 11);
}

TEST_F(task_manager_tests, test_start_tasks_with_dependency) {
    //Set up
    task_manager test_subject;
    uint8_t test_value = 10;

    //Test
    task_id id1 = test_subject.start_task(new ST::task(test_task_function2, &test_value, nullptr));
    task_id id2 = test_subject.start_task(new ST::task(test_task_function, &test_value, id1));

    test_subject.wait_for_task(id2);
    ASSERT_EQ(test_value, 12);
}

TEST_F(task_manager_tests, test_start_task_lockfree_without_dependency) {
    //Set up
    task_manager test_subject;
    uint8_t test_value = 10;

    //Test
    test_subject.start_task_lockfree(new ST::task(test_task_function, &test_value, nullptr));

    //Wait a suffecent amount of time
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ASSERT_EQ(test_value, 11);
}

TEST_F(task_manager_tests, test_start_task_lockfree_with_dependency) {
    //Set up
    task_manager test_subject;
    uint8_t test_value = 10;

    //Test
    task_id id1 = test_subject.start_task(new ST::task(test_task_function2, &test_value, nullptr));
    test_subject.start_task_lockfree(new ST::task(test_task_function, &test_value, id1));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    ASSERT_EQ(test_value, 12);
}

//With only one task thread running the test will take about 2 seconds to complete if
//the waiter isn't doing any work
TEST_P(task_manager_tests, test_do_work_while_waiting) {
    //Set up
    task_manager test_subject(1);
    uint8_t test_value1 = 10;
    uint8_t test_value2 = 20;

    //Test
    auto start = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch());

    task_id id1 = test_subject.start_task(new ST::task(test_task_function2, &test_value1, nullptr));
    task_id id2 = test_subject.start_task(new ST::task(test_task_function2, &test_value2, nullptr));

    test_subject.work_wait_for_task(id1);
    test_subject.work_wait_for_task(id2);
    auto end = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch());

    ASSERT_NEAR(static_cast<double>(end.count() - start.count()), 515, 200);
    ASSERT_EQ(test_value1, 11);
    ASSERT_EQ(test_value2, 21);
}

INSTANTIATE_TEST_CASE_P(test_do_work_while_waiting, task_manager_tests, ::testing::Range(0, 15));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}