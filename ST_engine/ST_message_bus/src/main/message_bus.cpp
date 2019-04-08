/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <message_bus.hpp>
#include <ST_util/pool_allocator_256.hpp>

/**
 * The allocator is declared globally.
 */
ST::pool_allocator_256<message> msg_memory;

//message_bus implementation=====================================================

/**
 * Sends a message to all subscribers of that message type.
 * Creates a copy of the message if it has more than one subscriber.
 */
void message_bus::send_msg(message* arg){
    uint8_t name = arg->msg_name;
    std::vector<subscriber*>* temp = &subscribers[name];

    //Locks aren't really needed here as there won't be any new subscribers in the middle of the game
    //(if you do want to have subsystems subscribe at random times you should definitely add locks)

    if(!temp->empty()){
        temp->at(0)->push_message(arg);
        for(uint32_t i = 1; i < temp->size(); i++){
            temp->at(i)->push_message(arg->make_copy()); //yes all queues are thread-safe so this is fine
        }
    }
}

static bool singleton_initialized = false;

/**
 * Default constructor for the message bus.
 * Throws exception if initialized twice.
 */
message_bus::message_bus() {
    if(singleton_initialized){
        throw std::runtime_error("The message bus cannot be initialized more than once!");
    }else{
        singleton_initialized = true;
    }
}

message_bus::~message_bus() {
    subscribers.clear();
    singleton_initialized = false;
}

void message_bus::clear() {
    subscribers.clear();
}

/**
 * Subscribe to a message type - adds the subscriber object to the list of subscribers for the given message type.
 * @param msg The type of the message.
 * @param sub The subscriber object.
 */
void message_bus::subscribe(uint8_t msg, subscriber* sub){
    subscribers[msg].push_back(sub);
}

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
message* make_msg(uint8_t name, const std::shared_ptr<void>& data){
    message* temp = msg_memory.allocate();
    temp->msg_name = name;
    temp->set_data(data);
    return temp;
}

/**
 * Destroys a message. Call this when you absolutely no longer need the message.
 * @param msg The message to destroy.
 */
void destroy_msg(message* msg){
    msg_memory.deallocate(msg);
}