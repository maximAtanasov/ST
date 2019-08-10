#ifndef ST_LINEAR_FRAME_ALLOCATOR_256_HPP
#define ST_LINEAR_FRAME_ALLOCATOR_256_HPP

#include <atomic>

/**
 * Not a real allocator.
 * Simply returns a chunk of memory that is
 * guaranteed to be valid for the current frame for up to 256 objects of type T.
 * Memory is not freed. Once all of it is used up, we start again at the start the block.
 * (Hence the 256 objects per frame limit).
 * ST::message and ST::task use this "allocation" strategy. This is very fast, lock-free and accessible
 * from multiple threads.
 */
namespace ST {
    template <class T> class linear_frame_allocator_256 {

    private:
        T memory[256];
        std::atomic_uint8_t pointer{0};

    public:
        T* allocate(){
            return &memory[pointer++];
        }
    };
}

#endif //ST_LINEAR_FRAME_ALLOCATOR_256_HPP
