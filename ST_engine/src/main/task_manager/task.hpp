/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef TASK_DEF
#define TASK_DEF

#include <SDL/SDL_mutex.h>

namespace ST {

    ///An object representing a task to be run by the task manager.
    /**
     * Contains a function pointer.
     * Data to pass to that function pointer.
     * And some locking mechanisms.
     */
    class task {
    public:
        void (*task_func)(void *);
        void *data;
        SDL_semaphore *lock{};
        SDL_semaphore *dependency{};
        bool has_lock = false;
        int8_t affinity = -1;
        const uint16_t id{};

        /**
         *
         * @param id An id given to each task by the task allocator
         * @param function A function representing a work task
         * @param arg The arguments to above function
         * @param dependency A lock acting as a dependency to the task or nullptr if there is no such dependency
         * @param affinity Thread affinity for the task
         */
        task(uint16_t id, void (*task_func)(void *), void *data, SDL_semaphore* dependency, int8_t affinity) : id(id){
            this->task_func = task_func;
            this->data = data;
            this->dependency = dependency;
            this->affinity = affinity;
        }

        void set_lock(SDL_semaphore *lock) {
            has_lock = static_cast<bool>(lock);
            this->lock = lock;
        }
    };
}

#endif
