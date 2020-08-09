/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <main/fps.hpp>

/**
 * Updates the fps counter with the current time and current frame_time.
 * @param time The current time.
 * @param frame_time The current frame time.
 */
void fps::update(double time, double frame_time) {
    average[counter++] = frame_time;
    counter = (counter != 16)*counter;
    new_time = time;
    if(new_time - old_time > 64){
        value = get_average();
        old_time = new_time;
    }
}

/**
 * @return The average framerate.
 */
double fps::get_average() {
    double sum = 0;
    for(double i : average){
        sum += i;
    }
    return sum/16;
}