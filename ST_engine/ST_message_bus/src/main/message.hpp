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
#include <ST_util/pool_allocator_256.hpp>

///A message object passed around in the message bus. Holds anything created with make_data<>().
/**
 * Only use new message() and delete() for the creation of a message.
 */
class message{
private:
    static ST::pool_allocator_256<message> allocator;
    std::shared_ptr<void> data; //yes, this holds anything created with make_data<>() AND calls the correct destructor
    //that's how shared_ptr works, if you don't believe me, well google it or something

public:
    uint8_t msg_name{};
    void* get_data() const;
    void set_data(const std::shared_ptr<void>& data);
    message *make_copy();

    message() = default;

    /**
     * @param name The type of message. See <b>ST::msg_type</b>.
     * @param data The data the message carries - created with <b>make_data<>()</b> or is <b>nullptr</b>
     */
    message(uint8_t name, const std::shared_ptr<void>& data){
        this->msg_name = name;
        this->data = data;
    }

    void* operator new (std::size_t count){
        return allocator.allocate();
    }

    void operator delete (void* ptr){
        allocator.deallocate(static_cast<message*>(ptr));
    }

};

//INLINED METHODS

/**
 * @return The data contained within a message. Always a void* that MUST be properly casted to an actual type on the other end
 */
inline void* message::get_data() const{
    return this->data.get();
}

inline void message::set_data(const std::shared_ptr<void>& data_) {
    this->data = data_;
}

inline message* message::make_copy() {
    return new message(this->msg_name, this->data);
}

#endif //ST_MESSAGE_HPP
