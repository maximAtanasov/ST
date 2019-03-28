#ifndef ST_POOL_ALLOCATOR_256_HPP
#define ST_POOL_ALLOCATOR_256_HPP

#include <mutex>
#include <atomic>

namespace ST{
    template <class T> class pool_allocator_256 {

    private:
        std::mutex access_mutex;
        T memory[256];
        std::atomic_bool allocated[256]{};
        uint8_t memory_size = 255;
        uint8_t pointer = 0;

    public:

        pool_allocator_256(){
            for(uint16_t i = 0; i < memory_size; i++){
                allocated[i] = false; //mark all memory as free
            }
        }

        T* allocate(){
            uint8_t pointer_temp;

            access_mutex.lock();
            //find the next free spot in memory
            while(allocated[pointer]){
                ++pointer;
            }
            pointer_temp = pointer;
            allocated[pointer++] = true;
            access_mutex.unlock();

            memory[pointer_temp].id = pointer_temp;
            return &memory[pointer_temp];
        }

        void deallocate(uint8_t id){
            allocated[id] = false;
        }
    };
}


#endif //ST_POOL_ALLOCATOR_256_HPP
