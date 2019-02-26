/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef TASK_DEF
#define TASK_DEF

#include "semaphore.hpp"

class task_allocator;
extern task_allocator gTask_allocator;

namespace ST {

    ///An object representing a task to be run by the task manager.
    /**
     * Contains a function pointer.
     * Data to pass to that function pointer.
     * And some locking mechanisms.
     */
    class task {

    public:
        void (*task_func)(void *){};

        void* data{};
        semaphore* lock = nullptr;
        semaphore* dependency{};

        inline uint8_t get_id() const{
            return id;
        }

    private:
        friend class ::task_allocator;
        uint8_t id = 0;
    };
}

#endif
