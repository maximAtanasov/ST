#include <main/timer.hpp>

int timer::initialize(){
    start = std::chrono::high_resolution_clock::now();
    return 0;
}
