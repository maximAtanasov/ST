/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef SLAVIC_TALES_MESSAGE_HPP
#define SLAVIC_TALES_MESSAGE_HPP


#include <memory>

class message_allocator;
extern message_allocator msg_memory;

///A message object passed around in the message bus. Holds anything created with make_data<>().
/**
 * Only use make_msg() and destroy_msg() for the creation of a message.
 */
class message{
private:
    friend class message_allocator;
    std::shared_ptr<void> data; //yes, this holds anything created with make_data<>() AND calls the correct destructor
    //that's how shared_ptr works, if you don't believe me, well google it or something
    uint8_t id; //used during allocation and de-allocation

public:
    uint8_t msg_name;
    uint8_t get_id() const;
    void* get_data() const;
    message* make_copy() const;
};

//INLINED METHODS

/**
 * @return The data contained within a message. Always a void* that MUST be properly casted to an actual type on the other end
 */
inline void* message::get_data() const{
    return this->data.get();
}

/**
 * @return The id of the message.
 */
inline uint8_t message::get_id() const{
    return id;
}

#endif //ST_MESSAGE_HPP
