/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVICTALES_MASTER_MATH_HPP
#define SLAVICTALES_MASTER_MATH_HPP

#include <chrono>
#include <random>

namespace ST {

    //abs functions
    inline int m_abs(int value) {
        auto temp = static_cast<unsigned int>(value >> 31);
        value ^= temp;
        value += temp & 1;
        return value;
    }

    //mod function - only for positive integers (and yes, it is faster than %, I've tested it)
    inline size_t pos_mod(size_t a, size_t b) {
        return a >= b ? a % b : a;
    }

    inline int random_int(int max)
    {
        static long seed(std::chrono::system_clock::now().time_since_epoch().count());
        static std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dis(0, max - 1);
        return dis(gen);
    }

}

#endif //SLAVICTALES_MASTER_MATH_HPP
