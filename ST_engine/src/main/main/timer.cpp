/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <main/timer.hpp>

/**
 * Initializes the timer with the current time.
 */
timer::timer(){
    start = std::chrono::high_resolution_clock::now();
}
