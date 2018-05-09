/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef TIMER_DEF
#define TIMER_DEF

#include <chrono>

///A timer used for keeping track of time in the main loop.
/**
 * Uses a std::chrono::high_resolution_clock internally to provide quite accurate timekeeping.
 */
class timer{
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    public:
        timer() = default;
        int initialize();
        double time_since_start();
};

//INLINED METHODS

/**
 *
 * @return The elapsed time since the initialization of the timer.
 */
inline double timer::time_since_start(){
    std::chrono::duration<double> elapsed_seconds = std::chrono::high_resolution_clock::now() - start;
    return elapsed_seconds.count() * 1000;
}

#endif
