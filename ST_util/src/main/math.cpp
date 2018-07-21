/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <ST_util/math.hpp>

int ST::random_int(int max){
    static long seed(std::chrono::system_clock::now().time_since_epoch().count());
    static std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(0, max - 1);
    return dis(gen);
}