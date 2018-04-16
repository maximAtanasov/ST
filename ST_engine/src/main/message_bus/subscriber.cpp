/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <message_bus/subscriber.hpp>

message* subscriber::get_next_message(){
    message* new_message;
    if(queue.try_dequeue(new_message)){
        return new_message;
    }else {
        return nullptr;
    }
}

//and again, the queues are thread-safe so no locks are needed
void subscriber::push_message(message* arg){
    queue.enqueue(arg);
}