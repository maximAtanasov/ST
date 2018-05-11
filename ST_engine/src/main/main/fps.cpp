/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <main/fps.hpp>

/**
 * Initializes the fps counter.
 * @return Always 0.
 */
fps::fps() {
    for(double& i : average) {
        i = 0;
    }
}

/**
 * Updates the fps counter with the current time and current frame_time.
 * @param time The current time.
 * @param frame_time The current frame time.
 */
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

/**
 * @return The average framerate of the last second.
 */
double fps::get_average() {
    double sum = 0;
    for(double i : average){
        sum += i;
    }
    return sum/60;
}