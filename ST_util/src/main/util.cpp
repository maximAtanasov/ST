/* This file is part of the "slavicTales" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: atanasovmaksim1@gmail.com
 */

#include <cstdlib>
#include <cstddef>
#include <ST_util/debug_switch.hpp>
#include <cstdio>

#ifdef __DEBUG

void* operator new(size_t size){
    return malloc(size);
}

void operator delete(void* ptr){
    free(ptr);
}

void* operator new[](size_t size){
    return malloc(size);
}

void operator delete[](void* ptr){
    free(ptr);
}

#endif