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
#include <ST_util/linear_frame_allocator_256.hpp>

namespace ST {

    ///An object representing a task to be run by the task manager.
    /**
     * Contains a function pointer.
     * Data to pass to that function pointer.
     * And some locking mechanisms.
     */
    class task {
    private:
        static linear_frame_allocator_256<task> allocator;

    public:

        void (*task_func)(void *){};

        void* data{};
        semaphore* lock = nullptr;
        semaphore* dependency{};

        task() = default;

        /**
         * @param function A function representing a work task
         * @param arg The arguments to above function
         * @param dependency A lock acting as a dependency to the task or nullptr if there is no such dependency
         */
        task(void (*function)(void *), void *arg, semaphore *dependency){
            this->task_func = function;
            this->data = arg;
            this->dependency = dependency;
        }

        void* operator new (std::size_t count){
            return allocator.allocate();
        }

        void operator delete (void* ptr){}
    };
}

#endif
