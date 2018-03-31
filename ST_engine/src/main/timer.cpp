#include "timer.hpp"

int timer::initialize(){
    start = std::chrono::high_resolution_clock::now();
    return 0;
}

double timer::time_since_start(){
    std::chrono::duration<double> elapsed_seconds = std::chrono::high_resolution_clock::now() - start;
    return elapsed_seconds.count() * 1000;
}