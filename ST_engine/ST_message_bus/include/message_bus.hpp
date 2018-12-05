/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef MSG_BUS_DEF
#define MSG_BUS_DEF

#include <cstdlib>
#include <iostream>
#include <ST_util/atomic_queue/concurrentqueue.h>
#include "../src/main/message.hpp"
#include <ST_util/bytell_hash_map.hpp>
#include <memory>
#include <vector>
#include <cstring>
#include "../src/main/message_allocator.hpp"
#include "../src/main/subscriber.hpp"

///The centeral messaging system of the engine. All subsystem make extensive use of it.
/**
 *
 * Handles all passing of messages to subscribers.
 */
class message_bus{
    private:
        friend class message_bus_tests;
        ska::bytell_hash_map<int, std::vector<subscriber*>> subscribers; //each message enum maps to a list of subscribers for that message
    public:
        message_bus();
        ~message_bus();
        void send_msg(message* msg);
        void subscribe(int msg, subscriber* sub);
};

/**
 * Creates a new message object given the type of the message and data.
 * To create a message use "make_msg" along with "make_data<>" - you do not need to manage the memory of the data only the lifetime of a message.
 * Any data you get out of a message (using get_data()) is guaranteed to be available until you
 * call "destroy_msg()" on that message, afterwards the results are undefined.
 * Shared pointers are used internally to manage memory.
 * @param name The type of message. See <b>ST::msg_type</b>.
 * @param data The data the message carries - created with <b>make_data<>()</b> or is <b>nullptr</b>
 * @return A new message object.
 */
inline message* make_msg(int name, const std::shared_ptr<void>& data){
    return msg_memory.allocate_message(name, data);
}

/**
 * Destroys a message. Call this when you absolutely no longer need the message.
 * @param msg The message to destroy.
 */
inline void destroy_msg(message* msg){
    msg_memory.deallocate(msg->get_id());
}

/**
 * Makes a copy of itself.
 * @return A new message that is an exact copy of the original.
 */
inline message* message::make_copy() const{
    return msg_memory.allocate_message(this->msg_name, this->data);
}

/**
 * Really just a wrapper around the cumbersome - static_cast<std::shared_ptr<void>>(std::make_shared<T>(data));
 * @tparam T The type of the data.
 * @param data The data itself - usually passed by value.
 * @return A new std::shared_ptr<void> representing the data.
 */
template <class T> std::shared_ptr<void> make_data(T data){
    return std::make_shared<T>(data);
}

#endif
