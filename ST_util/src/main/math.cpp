/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <ST_util/math.hpp>

int ST::random_int(int max){
    static long seed(std::chrono::system_clock::now().time_since_epoch().count());
    static std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(0, max - 1);
    return dis(gen);
}