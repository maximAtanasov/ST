/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVIC_TALES_MATH_HPP
#define SLAVIC_TALES_MATH_HPP

#include <chrono>
#include <random>

namespace ST {

    //abs functions
    inline int abs(int value) {
        auto temp = static_cast<unsigned int>(value >> 31);
        value ^= temp;
        value += temp & 1;
        return value;
    }

    //mod function - only for positive integers (and yes, it is faster than %, I've tested it)
    inline size_t pos_mod(size_t a, size_t b) {
        return a >= b ? a % b : a;
    }


    int random_int(int max);
}

#endif //SLAVIC_TALES_MATH_HPP
