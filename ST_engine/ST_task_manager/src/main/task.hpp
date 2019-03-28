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
#include <ST_util/pool_allocator_256.hpp>

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

        task() = default;

        task(void (*function)(void *), void *arg, semaphore *dependency){
            this->task_func = function;
            this->data = arg;
            this->dependency = dependency;
        }

    private:
        friend pool_allocator_256<ST::task>;
        uint8_t id = 0;
    };
}

#endif
