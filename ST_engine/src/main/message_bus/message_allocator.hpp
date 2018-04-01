#ifndef SLAVICTALES_MASTER_MESSAGE_ALLOCATOR_HPP
#define SLAVICTALES_MASTER_MESSAGE_ALLOCATOR_HPP


#include "message.hpp"
#include "message_types.hpp"
#include "../defs.hpp"

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
    void deallocate(int id, message* temp);
    ~message_allocator();
};


#endif //SLAVICTALES_MASTER_MESSAGE_ALLOCATOR_HPP
