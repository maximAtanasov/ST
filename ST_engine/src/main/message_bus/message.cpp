#include <message_bus/message.hpp>
#include <message_bus/message_allocator.hpp>

//message implementation=========================================================

//allocator and constructor for messages
message* make_msg(msg_type name, const std::shared_ptr<void>& data){
    return msg_memory->allocate_message(name, data);
}

void destroy_msg(message* msg){
    msg_memory->deallocate(msg->get_id(), msg);
}

//constructor - only ever used by the message_allocator class
message::message(msg_type name, const std::shared_ptr<void>& data, int id){
    msg_name = name;
    this->data = data;
    this->id = id;
}

void* message::get_data(){
    return this->data.get();
}

int message::get_id(){
    return id;
}

message* message::make_copy(){
    return msg_memory->allocate_message(this->msg_name, this->data);
}