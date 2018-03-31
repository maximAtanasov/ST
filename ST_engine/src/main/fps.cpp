#include "fps.hpp"

int fps::initialize() {
    for(double& i : average) {
        i = 0;
    }
    return 0;
}

void fps::update(double time, double frame_time) {
    average[counter++] = frame_time;
    if(counter == 60){
        counter = 0;
    }
    new_time = time;
    if(new_time - old_time > 1000){
        value = get_average();
        old_time = new_time;
    }
}

double fps::get_value() {
    return value;
}

double fps::get_average() {
    double sum = 0;
    for(double i : average){
        sum += i;
    }
    return sum/60;
}