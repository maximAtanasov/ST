/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef MSG_BUS_DEF
#define MSG_BUS_DEF

#include <cstdlib>
#include <iostream>
#include <util/atomic_queue/concurrentqueue.h>
#include <message_bus/subscriber.hpp>
#include <message_bus/message.hpp>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <cstring>
#include <message_bus/message_allocator.hpp>

class message_bus{
    private:
        std::unordered_map<int, std::vector<subscriber*>> subscribers; //each message enum maps to a list of subscribers for that message
    public:
        message_bus() = default;
        int initialize();
        void send_msg(message* msg);
        void subscribe(msg_type msg, subscriber* sub);
        void close();
};

//To create a message use "make_msg" along with "make_data<>" - you do not need to manage the memory of the data
//only the lifetime of a message
//Any data you get out of a message (using get_data()) is guaranteed to be available until you call "destroy_msg()" on that message
//afterwards the results are undefined
//shared pointer are used internally to manage memory
message* make_msg(msg_type name, const std::shared_ptr<void>& data);
void destroy_msg(message* msg);

//allocator and constructor for messages
inline message* make_msg(msg_type name, const std::shared_ptr<void>& data){
    return msg_memory.allocate_message(name, data);
}

inline void destroy_msg(message* msg){
    msg_memory.deallocate(msg->get_id());
}

inline message* message::make_copy(){
    return msg_memory.allocate_message(this->msg_name, this->data);
}

template <class T> std::shared_ptr<void> make_data(T data){
    return std::make_shared<T>(data);
}

#endif
