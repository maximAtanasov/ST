/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_SUBSCRIBER_HPP
#define SLAVIC_TALES_SUBSCRIBER_HPP

#include <message_bus/message.hpp>
#include <util/atomic_queue/concurrentqueue.h>

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
