/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_SUBSCRIBER_HPP
#define SLAVIC_TALES_SUBSCRIBER_HPP

#include <message_bus/message.hpp>
#include <ST_util/atomic_queue/concurrentqueue.h>

///This class handles a small queue for messages.
/**
 * A simple wrapper around  moodycamel::ConcurrentQueue<message*>.
 */
class subscriber{
private:
    moodycamel::ConcurrentQueue<message*> queue;
public:
    subscriber() = default;
    message* get_next_message();
    void push_message(message* arg);
};

#endif //SLAVIC_TALES_SUBSCRIBER_HPP
