/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_POOL_ALLOCATOR_256_HPP
#define ST_POOL_ALLOCATOR_256_HPP

#include <mutex>
#include <atomic>

namespace ST {
    template<class T>
    class pool_allocator_256 {

    private:
        std::mutex access_mutex;
        T memory[256];
        std::atomic_bool allocated[256]{};
        const uint8_t memory_size = 255;
        uint8_t pointer = 0;

    public:

        pool_allocator_256() {
            for (uint16_t i = 0; i < memory_size; ++i) {
                allocated[i] = false; //mark all memory as free
            }
        }

        T *allocate() {
            uint8_t pointer_temp;

            access_mutex.lock();
            //find the next free spot in memory
            while (allocated[++pointer]);
            pointer_temp = pointer;
            allocated[pointer] = true;
            access_mutex.unlock();
            return &memory[pointer_temp];
        }

        void deallocate(T *mem_location) {
            allocated[(mem_location - memory)] = false;
        }
    };
}

#endif //ST_POOL_ALLOCATOR_256_HPP
