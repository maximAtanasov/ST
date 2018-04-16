/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef TASK_DEF
#define TASK_DEF

namespace ST {

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
