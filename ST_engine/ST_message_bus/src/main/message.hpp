/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_MESSAGE_HPP
#define ST_MESSAGE_HPP


#include <memory>
#include <ST_util/linear_frame_allocator_256.hpp>

///A message object passed around in the message bus. Holds anything created with make_data<>().
/**
 * Only use new message() and delete() for the creation of a message.
 */
class message{
private:
    static ST::linear_frame_allocator_256<message> allocator;
    std::shared_ptr<void> data; //yes, this holds anything created with make_data<>() AND calls the correct destructor

public:
    //An additional 48 bits (3 bytes) of data can be stored here. We're 8 byte aligned, so this is for free.
    uint32_t base_data0 = 0; //additional 32 bits
    uint16_t base_data1 = 0; //additional 16 bits
    uint8_t base_data2 = 0; //additional 8 bits

    uint8_t msg_name{};

    void* get_data() const;
    message *make_copy();

    message() = default;

    /**
     * @param name The type of message. See <b>ST::msg_type</b>.
     * @param data The data the message carries - created with <b>make_data<>()</b> or is <b>nullptr</b>
     */
    explicit message(uint8_t name, const std::shared_ptr<void>& data = nullptr){
        this->msg_name = name;
        this->data = data;
    }

    /*
     * @param name The type of message. See <b>ST::msg_type</b>.
     * @param base_data0 32 bits of data. Use this if you want to avoid creating a shared pointer.
     * @param data The data the message carries - created with <b>make_data<>()</b> or is <b>nullptr</b>
     */
    message(uint8_t name, uint32_t base_data0, const std::shared_ptr<void>& data = nullptr){
        this->msg_name = name;
        this->base_data0 = base_data0;
        this->data = data;
    }

    void* operator new (std::size_t count){
        return allocator.allocate();
    }

    void operator delete (void* ptr){}

    ~message(){
        this->data.reset();
    }

};

static_assert(sizeof(message) == 24, "sizeof message is not 24");
//INLINED METHODS

/**
 * @return The data contained within a message. Always a void* that MUST be properly cast to an actual type on the other end
 */
inline void* message::get_data() const{
    return this->data.get();
}

/**
 * @return A copy of this message.
 */
inline message* message::make_copy() {
    return &(*allocator.allocate() = *this);
}

#endif //ST_MESSAGE_HPP
