/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <message_bus/message_bus.hpp>
#include <message_bus/message_allocator.hpp>

/**
 * The allocator is declared globaly.
 */
message_allocator msg_memory{};

//message_bus implementation=====================================================

/**
 * Sends a message to all subscribers of that message type.
 * Creates a copy of the message if it has more than one subscriber.
 */
void message_bus::send_msg(message* arg){
    int name = arg->msg_name;
    std::vector<subscriber*>* temp = &subscribers[name];

    //Locks aren't really needed here as there won't be any new subscribers in the middle of the game
    //(if you do want to have subsystems subscribe at random times you should definitely add locks)

    if(temp->size() == 1){
        temp->at(0)->push_message(arg);
    }
    else{
        for(auto i : *temp){
            i->push_message(arg->make_copy()); //yes all queues are thread-safe so this is fine
        }
        destroy_msg(arg);
    }
}

/**
 * Subscribe to a message type - adds the subscriber object to the list of subscribers for the given message type.
 * @param msg The type of the message.
 * @param sub The subscriber object.
 */
void message_bus::subscribe(msg_type msg, subscriber* sub){
    int temp = msg;
    subscribers[temp].push_back(sub);
}