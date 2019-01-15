/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef SLAVIC_TALES_SUBSCRIBER_HPP
#define SLAVIC_TALES_SUBSCRIBER_HPP

#include <ST_util/atomic_queue/concurrentqueue.h>
#include "message.hpp"

///This class handles a small queue for messages.
/**
 * A simple wrapper around  moodycamel::ConcurrentQueue<message*>.
 */
class subscriber{
public:
    subscriber() = default;
    message* get_next_message();
    void push_message(message* arg);
private:
    moodycamel::ConcurrentQueue<message*> queue;
};

#endif //SLAVIC_TALES_SUBSCRIBER_HPP
