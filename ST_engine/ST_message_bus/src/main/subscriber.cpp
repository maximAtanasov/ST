/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <subscriber.hpp>

/**
 * Get the next message in the subscription queue.
 * @return The next message or nullptr if nothing was found.
 */
message* subscriber::get_next_message(){
    message* new_message;
    if(queue.try_dequeue(new_message)){
        return new_message;
    }else {
        return nullptr;
    }
}

/**
 * Pushes a new message to the message_bus.
 * @param arg The message object to push.
 */
void subscriber::push_message(message* arg){
    queue.enqueue(arg);
    //and again, the queues are thread-safe so no locks are needed
}