#ifndef TASK_MNGR_DEF
#define TASK_MNGR_DEF

#include <defs.hpp>
#include <message_bus/message_bus.hpp>
#include <task_manager/task.hpp>

//The task manager only needs to be initialized, it will start at least one worker thread
//and an update thread that will distribute incoming tasks

typedef SDL_semaphore* task_id;

class task_manager{

    private:
        int thread_num = 0;
        message_bus* gMessage_bus{};
        std::vector<SDL_Thread*> task_threads{};
        SDL_atomic_t run_threads{};
        SDL_semaphore* work_sem{};
        moodycamel::ConcurrentQueue<task*> task_queue;

        static int task_thread(void* arg);
        void do_work(task* work);
        void start_thread(int (*thread_func)(void*), void* data);

    public:
        task_manager() = default;
        void close();
        int initialize(message_bus* msg_bus);
		task_id start_task(task* arg);
        void start_task_lockfree(task* arg);
        void wait_for_task(task_id id);
};

#endif
