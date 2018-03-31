#ifndef TIMER_DEF
#define TIMER_DEF

#include <chrono>

class timer{
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    public:
        timer() = default;
        int initialize();
        double time_since_start();
};

#endif
