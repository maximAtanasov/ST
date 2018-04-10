#ifndef SLAVICTALES_MASTER_SUBSCRIBER_HPP
#define SLAVICTALES_MASTER_SUBSCRIBER_HPP

#include <message_bus/message.hpp>
#include <util/atomic_queue/concurrentqueue.h>

class subscriber{
private:
    moodycamel::ConcurrentQueue<message*> queue;
public:
    subscriber() = default;
    message* get_next_message();
    void push_message(message* arg);
};

#endif //SLAVICTALES_MASTER_SUBSCRIBER_HPP
