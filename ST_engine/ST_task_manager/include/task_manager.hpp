/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef TASK_MNGR_DEF
#define TASK_MNGR_DEF

#include <message_bus.hpp>
#include "../src/main/task.hpp"
#include "../src/main/semaphore.hpp"

typedef semaphore* task_id;

///The Task Manager handles all things multi-threaded in the engine.
/**
 * The task manager only needs to be initialized, it will start at least one worker thread
 *
 */
class task_manager{

    private:
        uint8_t thread_num = 0;
        message_bus* gMessage_bus{};
        std::vector<std::thread> task_threads{};
        std::atomic_bool run_threads{true};
        semaphore* work_sem{};
        moodycamel::ConcurrentQueue<ST::task*> global_task_queue;

        static int task_thread(task_manager* self);
        static void do_work(ST::task* work);
        void start_thread(int (*thread_func)(void*), void* data);

    public:
        explicit task_manager(message_bus* msg_bus);
		task_manager(message_bus* msg_bus, uint8_t thread_num);
		~task_manager();
		task_id start_task(ST::task* arg);
        void start_task_lockfree(ST::task* arg);
        void wait_for_task(task_id id);
};

#endif //TASK_MNGR_DEF
