/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef SLAVIC_TALES_MATH_HPP
#define SLAVIC_TALES_MATH_HPP

#include <chrono>
#include <random>

namespace ST {

    //abs functions
    inline int abs(uint64_t value) {
        auto temp = static_cast<unsigned int>(value >> 31U);
        value ^= temp;
        value += temp & 1U;
        return value;
    }

    //mod function - only for positive integers (and yes, it is faster than %, I've tested it)
    inline size_t pos_mod(size_t a, size_t b) {
        return a >= b ? a % b : a;
    }


    int random_int(int max);
}

#endif //SLAVIC_TALES_MATH_HPP
