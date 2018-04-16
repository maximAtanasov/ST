/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVICTALES_MASTER_MESSAGE_ALLOCATOR_HPP
#define SLAVICTALES_MASTER_MESSAGE_ALLOCATOR_HPP


#include <message_bus/message.hpp>
#include <message_bus/message_types.hpp>
#include <defs.hpp>

class message_allocator{
private:
    SDL_mutex* access_mutex{};
    int pointer = 0;
    message* memory{};
    int memory_size = 300;
    bool allocated[300]{};

public:
    message_allocator();
    message* allocate_message(msg_type name, std::shared_ptr<void> data);
    void deallocate(int id);
    ~message_allocator();
};


#endif //SLAVICTALES_MASTER_MESSAGE_ALLOCATOR_HPP
