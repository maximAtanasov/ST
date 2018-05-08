/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_MESSAGE_HPP
#define SLAVIC_TALES_MESSAGE_HPP


#include <memory>
#include <message_bus/message_types.hpp>

class message_allocator;
extern message_allocator msg_memory;

///A message object passed around in the message bus. Holds anything created with make_data<>().
/**
 * Only use make_msg() and destroy_msg() for the creation of a message.
 */
class message{
private:
    friend class message_allocator;
    uint16_t id; //used during allocation and deallocation
    std::shared_ptr<void> data; //yes, this holds anything created with make_data<>() AND calls the correct destructor
    //that's how shared_ptr works, if you don't believe me, well google it or something

    message(msg_type name, const std::shared_ptr<void>& data, uint16_t id);
public:
    msg_type msg_name;
    uint16_t get_id();
    void* get_data();
    message* make_copy();
};

//INLINED METHODS

//message implementation=========================================================

/**
 * constructor - only ever used by the message_allocator class.
 */
inline message::message(msg_type name, const std::shared_ptr<void>& data, uint16_t id){
    msg_name = name;
    this->data = data;
    this->id = id;
}

/**
 * @return The data contained within a message. Always a void* that MUST be properly casted to an actual type on the other end
 */
inline void* message::get_data(){
    return this->data.get();
}

/**
 * @return The id of the mesage.
 */
inline uint16_t message::get_id(){
    return id;
}

#endif //SLAVIC_TALES_MESSAGE_HPP
