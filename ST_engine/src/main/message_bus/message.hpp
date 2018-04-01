#ifndef SLAVICTALES_MASTER_MESSAGE_HPP
#define SLAVICTALES_MASTER_MESSAGE_HPP


#include <memory>
#include "message_types.hpp"

class message_allocator;
extern message_allocator* msg_memory;

//Never use new() or delete() or the constructor  (it will just break) with this class - only make_msg() and destroy_msg()
class message{
private:
    int id; //used during allocation and deallocation
    std::shared_ptr<void> data; //yes, this holds anything created with make_data<>() AND calls the correct destructor
    //that's how shared_ptr works, if you don't believe me, well google it or something
public:
    msg_type msg_name;
    int get_id();
    message(msg_type name, const std::shared_ptr<void>& data, int id);
    void* get_data();
    message* make_copy();
};


#endif //SLAVICTALES_MASTER_MESSAGE_HPP
