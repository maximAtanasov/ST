#include <utility>

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
    for(uint16_t i = 0; i < memory_size+1; ++i){
        allocated[i] = false;
    }
}

/**
 * Creates a new message using pre-allocated memory.
 * @param name The message type.
 * @param data The message data.
 * @return The new message.
 */
message* message_allocator::allocate_message(uint8_t name, std::shared_ptr<void> data){
    access_mutex.lock();
    while(allocated[pointer++]); //find the next free spot in memory
    allocated[pointer] = true;
    auto pointer_temp = pointer;
    access_mutex.unlock();
    memory[pointer_temp].data = std::move(data);
    memory[pointer_temp].msg_name = name;
    memory[pointer_temp].id = pointer_temp;
    return &memory[pointer_temp];
}
