/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <message_bus.hpp>

/**
 * The allocator is declared globally.
 */
message_allocator msg_memory{};

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