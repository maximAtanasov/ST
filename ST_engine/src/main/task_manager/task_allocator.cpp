/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <task_manager/task_allocator.hpp>

/**
 * Constructor for the task allocator.
 * Allocates memory for tasks.
 */
task_allocator::task_allocator(){
    access_mutex = SDL_CreateMutex();
    pointer = 0;
    memory = static_cast<ST::task*>(malloc(sizeof(ST::task)*memory_size));
    for(uint32_t i = 0; i < memory_size; i++){
        allocated[i] = false; //mark all memory as free
    }
}

/**
 * Destructor for the allocator - frees all allocated memory.
 */
task_allocator::~task_allocator(){
    SDL_DestroyMutex(access_mutex);
    free(memory);
}

static task_allocator gTask_allocator{};

/**
 * //only use these functions to create/destroy tasks
 * @param function A function representing a work task
 * @param arg The arguments to above function
 * @param dependency A lock acting as a dependency to the task or nullptr if there is no such dependency
 * @param affinity Thread affinity for the task
 * @return A new task object
 */
ST::task* make_task(void (*function)(void *), void *arg, SDL_semaphore *dependency){
    return gTask_allocator.allocate_task(function, arg, dependency);
}

/**
 * Destroys a task using the task allocator
 * @param task The task to destroy
 */
void destroy_task(ST::task* task){
    gTask_allocator.deallocate(task->id);
}