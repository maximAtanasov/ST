/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef SLAVIC_TALES_MESSAGE_ALLOCATOR_HPP
#define SLAVIC_TALES_MESSAGE_ALLOCATOR_HPP


#include "message.hpp"
#include <mutex>
#include <atomic>

#define MESSAGE_ALLOCATOR_CAPACITY 256 //MUST be 256 ALWAYS

///An allocator class that pre-allocates memory for messages
/**
 * As messages are used a lot on every single frame, it makes sense
 * to pre-allocate them, which is what this class does.
 * It allocates enough for 300 messages - this can be adjusted, but
 * for the current needs of fhe engine it's enough.
 */
class message_allocator{
private:
    std::mutex access_mutex;
    uint8_t pointer = 0;
    message* memory{};
    const uint8_t memory_size = MESSAGE_ALLOCATOR_CAPACITY-1;
    std::atomic_bool allocated[MESSAGE_ALLOCATOR_CAPACITY]{};

public:
    message_allocator();
    message* allocate_message(int name, std::shared_ptr<void> data);
    void deallocate(uint8_t id);
    ~message_allocator();
};

/**
 * Deallocate a message.
 * Internally marks the previously used memory as free.
 * @param id The id of the message.
 */
inline void message_allocator::deallocate(uint8_t id){
    //with the help of the id we can mark the unused memory as free in our array
    allocated[id] = false;
}

#endif //SLAVIC_TALES_MESSAGE_ALLOCATOR_HPP
