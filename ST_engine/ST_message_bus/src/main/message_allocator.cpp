/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <SDL_mutex.h>
#include <memory>
#include <message_allocator.hpp>

/**
 * Constructor for the message allocator.
 * Allocates memory for messages.
 */
message_allocator::message_allocator(){
    access_mutex = SDL_CreateMutex();
    pointer = 0;
    memory = (message*)malloc(sizeof(message)*memory_size);
    for(uint32_t i = 0; i < memory_size; i++){
        allocated[i] = false; //mark all memory as free
    }
}

/**
 * Creates a new message using pre-allocated memory.
 * @param name The message type.
 * @param data The message data.
 * @return The new message.
 */
message* message_allocator::allocate_message(int name, std::shared_ptr<void> data){
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
    auto temp = new (memory+pointer) message(name, data, pointer);
    SDL_UnlockMutex(access_mutex);
    return temp;
}

/**
 * Deallocate a message.
 * Internally marks the previously used memory as free.
 * @param id The id of the message.
 */
void message_allocator::deallocate(uint16_t id){
    //with the help of the id we can mark the unused memory as free in our array
    SDL_LockMutex(access_mutex);
    allocated[id] = false;
    SDL_UnlockMutex(access_mutex);
}

/**
 * Destructor for the allocator - frees all allocated memory.
 */
message_allocator::~message_allocator(){
    SDL_DestroyMutex(access_mutex);
    free(memory);
}