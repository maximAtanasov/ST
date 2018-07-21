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
        int affinity = -1;

        task(void (*function)(void *), void *arg, SDL_semaphore *dependency, int affinity) {
            task_func = function;
            data = arg;
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
