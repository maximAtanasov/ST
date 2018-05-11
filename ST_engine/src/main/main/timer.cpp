/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <main/timer.hpp>

/**
 * Initializes the timer with the current time.
 */
timer::timer(){
    start = std::chrono::high_resolution_clock::now();
}
