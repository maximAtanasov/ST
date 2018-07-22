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
    memory = (ST::task*)malloc(sizeof(ST::task)*memory_size);
    for(int i = 0; i < memory_size; i++){
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

static task_allocator gTask_pool{};

/**
 * //only use these functions to create/destroy tasks
 * @param function A function representing a work task
 * @param arg The arguments to above function
 * @param dependency A lock acting as a dependency to the task or nullptr if there is no such dependency
 * @param affinity Thread affinity for the task
 * @return A new task object
 */
ST::task* make_task(void (*function)(void *), void *arg, SDL_semaphore *dependency, int affinity){
    return gTask_pool.allocate_task(function, arg, dependency, affinity);
}

/**
 * Destroys a task using the task allocator
 * @param task The task to destroy
 */
void destroy_task(ST::task* task){
    gTask_pool.deallocate(task->id);
}

//INLINED METHODS

inline ST::task* task_allocator::allocate_task(void (*function)(void *), void *arg, SDL_semaphore *dependency, int affinity){
    uint16_t i = 0;
    SDL_LockMutex(access_mutex);
    //find the next free spot in memory
    while(allocated[pointer] && i < memory_size){
        pointer++;
        i++;
        if(pointer > memory_size-1){
            pointer = 0;
        }
    }
    if(i == memory_size){
        return nullptr;
    }
    allocated[pointer] = true;
    auto temp = new (memory+pointer) ST::task(pointer, function, arg, dependency, affinity);
    SDL_UnlockMutex(access_mutex);
    return temp;
}

/**
 * Deallocate a task obbject.
 * Internally marks the previously used memory as free.
 * @param id The id of the task.
 */
inline void task_allocator::deallocate(uint16_t id){
    //with the help of the id we can mark the unused memory as free in our array
    SDL_LockMutex(access_mutex);
    allocated[id] = false;
    SDL_UnlockMutex(access_mutex);
}