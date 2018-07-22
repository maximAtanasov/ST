/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_MESSAGE_ALLOCATOR_HPP
#define SLAVIC_TALES_MESSAGE_ALLOCATOR_HPP


#include <message_bus/message.hpp>
#include <message_bus/message_types.hpp>
#include <defs.hpp>

#define MESSAGE_ALLOCATOR_CAPACITY 300

///An allocator class that pre-allocates memory for messages
/**
 * As messages are used a lot on every single frame, it makes sense
 * to pre-allocate them, which is what this class does.
 * It allocates enough for 300 messages - this can be adjusted, but
 * for the current needs of fhe engine it's enough.
 */
class message_allocator{
private:
    SDL_mutex* access_mutex{};
    uint16_t pointer = 0;
    message* memory{};
    uint32_t memory_size = MESSAGE_ALLOCATOR_CAPACITY;
    bool allocated[MESSAGE_ALLOCATOR_CAPACITY]{};

public:
    message_allocator();
    message* allocate_message(msg_type name, std::shared_ptr<void> data);
    void deallocate(uint16_t id);
    ~message_allocator();
};


#endif //SLAVIC_TALES_MESSAGE_ALLOCATOR_HPP
