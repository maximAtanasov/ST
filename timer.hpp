#ifndef TIMER_DEF
#define TIMER_DEF

#include "defs.hpp"

class timer{
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    public:
        timer();
        int initialize();
        
        inline double time_since_start(){
            std::chrono::duration<double> elapsed_seconds = std::chrono::high_resolution_clock::now() - start;
            return elapsed_seconds.count() * 1000;
        }
};

#endif
