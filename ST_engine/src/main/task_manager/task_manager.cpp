/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <task_manager/task_manager.hpp>
#include <console/log.hpp>

/**
 * The function each task thread runs.
 * @param arg A pointer to the task_manager.
 * @return Always 0. (This function only returns when at engine-shutdown).
 */
int task_manager::task_thread(void* arg){
	auto self = static_cast<task_manager*>(arg);
	ST::task* work;
	while(SDL_AtomicGet(&self->run_threads) == 1){
        SDL_SemWait(self->work_sem);
		if(self->task_queue.try_dequeue(work)){ //get a function pointer and data
            self->do_work(work);
		}
	}
    return 0;
}

/**
 * Runs a function pointer from a task with it's associated data.
 * Waits for any other tasks that are dependencies to the current one and waits for them.
 * @param work The ST::task object conatining job data.
 */
void task_manager::do_work(ST::task* work){
    if(work->dependency != nullptr){ //wait for dependency to finish
        SDL_SemWait(work->dependency);
        SDL_DestroySemaphore(work->dependency);
    }
    work->task_func(work->data); // call it
    if(work->has_lock) {
        SDL_SemPost(work->lock); //increment the semaphore
    }
    delete work;
}

/**
 * Initializes the task manager.
 * Starts as many worker threads as there are logical cores in the system - 1.
 * @param msg_bus A pointer to the global message bus.
 * @return Always 0.
 */
int task_manager::initialize(message_bus* msg_bus){

    //Set our external dependency
    gMessage_bus = msg_bus;

    SDL_AtomicSet(&run_threads, 1);

    //check how many threads we have
    thread_num = SDL_GetCPUCount();

    //initialize semaphore for worker threads
    work_sem = SDL_CreateSemaphore(0);

    log(INFO, "This system has " + std::to_string(thread_num) + " logical cores");

    //if we can't tell or there is only one core, then start one worker thread
    if(thread_num == 0 || thread_num == 1){
        thread_num = 2;
    }

	for (uint16_t i = 0; i < thread_num - 1; i++) {
		task_threads.emplace_back(SDL_CreateThread(this->task_thread, "tskThr", this));
	}
    return 0;
}

/**
 * This method will start a completely new thread, separate from the worker threads.
 * @param thread_func The function pointer.
 * @param data The data to suppply to it.
 */
void task_manager::start_thread(int (*thread_func)(void*), void* data){
    SDL_CreateThread(thread_func, " ", data);
}

/**
 * Closes the task manager.
 * Waits for all threads to exit properly and finishes any unfinished tasks.
 */
void task_manager::close(){
    SDL_AtomicSet(&run_threads, 0);
    for(int i = 0; i < thread_num-1; i++){
        SDL_SemPost(work_sem);
    }
    for(int i = 0; i < thread_num-1; i++) {
        SDL_WaitThread(task_threads[i], nullptr);
    }

    //finish running any remaining tasks
	ST::task* new_task;
	while(task_queue.try_dequeue(new_task)){ //get a function pointer and data
        SDL_DestroySemaphore(new_task->lock);
		delete new_task;
	}
    SDL_DestroySemaphore(work_sem);
}

//TODO: Task allocator

/**
 * Start a new task on one of the task threads.
 * @param arg The task object to use.
 * @return A task_id that can be used to wait for this task.
 */
task_id task_manager::start_task(ST::task* arg){
    //Aparrently a bit cheaper to create a new semaphore instead of reusing old ones
    //This may depend on the platform (OS implementation of Semaphores)
    SDL_semaphore *lock = SDL_CreateSemaphore(0);
    arg->set_lock(lock);
    task_queue.enqueue(arg);
    SDL_SemPost(work_sem);
    return lock;
}

/**
 * Same as start_task except no task_id (lock) is created and the task has no dependencies.
 * @param arg The task object to use.
 */
void task_manager::start_task_lockfree(ST::task* arg){
    arg->set_lock(nullptr);
    task_queue.enqueue(arg);
    SDL_SemPost(work_sem);
}

/**
 * Wait for a task to finish and do work from the work queue while waiting.
 * @param id The ID of the task.
 */
void task_manager::wait_for_task(task_id id){
    if(id != nullptr) {
        while(SDL_SemTryWait(id) != 0) {
            ST::task* work;
            if(SDL_AtomicGet(&run_threads) == 1){
                if(task_queue.try_dequeue(work)){ //get a function pointer and data
                    do_work(work);
                }
            }
        }
        SDL_DestroySemaphore(id);
    }
}