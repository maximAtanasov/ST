#include "timer.hpp"

timer::timer(){
    //empty constructor
}

int timer::initialize(){
    start = std::chrono::high_resolution_clock::now();
    return 0;
}
