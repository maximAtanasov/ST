/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef MSG_BUS_DEF
#define MSG_BUS_DEF

#include <cstdlib>
#include <iostream>
#include <ST_util/atomic_queue/concurrentqueue.h>
#include "../src/main/message.hpp"
#include <ST_util/bytell_hash_map.hpp>
#include <memory>
#include <vector>
#include <cstring>
#include "message_types.hpp"
#include "../src/main/subscriber.hpp"

///The central messaging system of the engine. All subsystem make extensive use of it.
/**
 *
 * Handles all passing of messages to subscribers.
 */
class message_bus {
private:
    friend class message_bus_tests;

    ska::bytell_hash_map<uint8_t, std::vector<subscriber *>> subscribers; //each message enum maps to a list of subscribers for that message
public:
    message_bus();

    ~message_bus();

    void clear();

    void send_msg(message *msg);

    void subscribe(uint8_t msg, subscriber *sub);
};

/**
 * Really just a wrapper around the cumbersome - static_cast<std::shared_ptr<void>>(std::make_shared<T>(data));
 * @tparam T The type of the data.
 * @param data The data itself - usually passed by value.
 * @return A new std::shared_ptr<void> representing the data.
 */
template<class T>
std::shared_ptr<void> make_data(T data) {
    return std::make_shared<T>(data);
}

#endif //MSG_BUS_DEF
