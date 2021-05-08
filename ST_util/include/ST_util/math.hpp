/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#ifndef ST_MATH_HPP
#define ST_MATH_HPP

#include <chrono>
#include <random>

namespace ST {

    //abs functions
    inline uint64_t abs(uint64_t value) {
        auto temp = static_cast<unsigned int>(value >> 31U);
        value ^= temp;
        value += temp & 1U;
        return value;
    }

    int random_int(int max);
}

#endif //ST_MATH_HPP
