/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVICTALES_TASK_ALLOCATOR_HPP
#define SLAVICTALES_TASK_ALLOCATOR_HPP


#define TASK_ALLOCATOR_CAPACITY 350


#include <defs.hpp>
#include <task_manager/task.hpp>

///An allocator for task objects
class task_allocator {
private:
    SDL_mutex* access_mutex{};
    uint16_t pointer = 0;
    ST::task* memory{};
    uint32_t memory_size = TASK_ALLOCATOR_CAPACITY;
    bool allocated[TASK_ALLOCATOR_CAPACITY]{};
public:
    ST::task* allocate_task(void (*function)(void *), void *arg, SDL_semaphore *dependency, int8_t affinity);
    void deallocate(uint16_t id);
    task_allocator();
    ~task_allocator();
};

//INLINED METHODS

inline ST::task* task_allocator::allocate_task(void (*function)(void *), void *arg, SDL_semaphore *dependency, int8_t affinity){
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
        fprintf(stderr, "More than %d tasks have been allocated, exiting!", TASK_ALLOCATOR_CAPACITY);
        exit(1);
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

/**
 * //only use these functions to create/destroy tasks
 * @param function A function representing a work task
 * @param arg The arguments to above function
 * @param dependency A lock acting as a dependency to the task or nullptr if there is no such dependency
 * @param affinity Thread affinity for the task
 * @return A new task object
 */
ST::task* make_task(void (*function)(void *), void *arg, SDL_semaphore *dependency, int8_t affinity);

/**
 * Destroys a task using the task allocator
 * @param task The task to destroy
 */
void destroy_task(ST::task* task);

#endif //SLAVICTALES_TASK_ALLOCATOR_HPP
