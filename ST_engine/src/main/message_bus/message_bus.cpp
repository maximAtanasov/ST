/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <message_bus/message_bus.hpp>
#include <message_bus/message_allocator.hpp>

message_allocator msg_memory{};

//message_bus implementation=====================================================

int message_bus::initialize(){
    return 0;
}

//send a message to all subscribers of that message type
//locks aren't really needed here as there won't be any new subscribers in the middle of the game
//(if you do want to have subsystems subscribe at random times you should definitely add locks)
void message_bus::send_msg(message* arg){
    int name = arg->msg_name;
    std::vector<subscriber*>* temp = &subscribers[name];
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

//subscribe to a message type -adds the subscriber object to the list of subscribers for the given message type
void message_bus::subscribe(msg_type msg, subscriber* sub){
    int temp = msg;
    subscribers[temp].push_back(sub);
}

void message_bus::close(){}
