#include "subscriber.hpp"

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