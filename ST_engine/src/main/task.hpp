#ifndef TASK_DEF
#define TASK_DEF

class task{
public:
    void (*task_func)(void*);
    void* data;
    SDL_semaphore* lock{};
    SDL_semaphore* dependency{};
    bool has_lock = false;
    int affinity = -1;

    task(void (*function)(void*), void* arg, SDL_semaphore* dependency, int affinity){
        task_func = function;
        data = arg;
        this->dependency = dependency;
        this->affinity = affinity;
    }

    void set_lock(SDL_semaphore* lock){
        has_lock = static_cast<bool>(lock);
        this->lock = lock;
    }

};

#endif
