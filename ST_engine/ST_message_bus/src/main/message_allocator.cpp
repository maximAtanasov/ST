/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <memory>
#include <message_allocator.hpp>

/**
 * Constructor for the message allocator.
 * Allocates memory for messages.
 */
message_allocator::message_allocator(){
    pointer = 0;
    memory = static_cast<message*>(malloc(sizeof(message)*memory_size));
    for(uint32_t i = 0; i < memory_size; ++i){
        allocated[i] = false; //mark all memory as free
        //TODO : Maybe use a bitfield instead of this array
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
    access_mutex.lock();
    //find the next free spot in memory
    while(allocated[pointer] && i < memory_size){
        ++pointer;
        ++i;
        if(pointer > memory_size-1){
            pointer = 0;
        }
    }
    if(i == memory_size){
        fprintf(stderr, "message bus: too many messages allocated, not enough memory, exiting\n");
        exit(1);
    }
    allocated[pointer] = true;
    uint16_t pointer_temp = pointer;
    access_mutex.unlock();
    return new (memory+pointer) message(name, data, pointer_temp);
}

/**
 * Destructor for the allocator - frees all allocated memory.
 */
message_allocator::~message_allocator(){
    free(memory);
}