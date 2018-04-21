/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project.
 *
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