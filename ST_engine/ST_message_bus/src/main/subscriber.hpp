/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_SUBSCRIBER_HPP
#define ST_SUBSCRIBER_HPP

#include <ST_util/atomic_queue/concurrentqueue.h>
#include "message.hpp"

///This class handles a small queue for messages.
/**
 * A simple wrapper around  moodycamel::ConcurrentQueue<message*>.
 */
class subscriber {
public:
    subscriber() = default;

    message *get_next_message();

    void push_message(message *arg);

private:
    moodycamel::ConcurrentQueue<message *> queue;
};


//INLINE METHODS

/**
 * Get the next message in the subscription queue.
 * @return The next message or nullptr if nothing was found.
 */
inline message *subscriber::get_next_message() {
    message *new_message{};
    queue.try_dequeue(new_message);
    return new_message;
}

/**
 * Pushes a new message to the message_bus.
 * @param arg The message object to push.
 */
inline void subscriber::push_message(message *arg) {
    queue.enqueue(arg);
    //and again, the queues are thread-safe so no locks are needed
}

#endif //ST_SUBSCRIBER_HPP
