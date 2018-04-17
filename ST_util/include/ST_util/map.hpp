/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef SLAVICTALES_MAP_HPP
#define SLAVICTALES_MAP_HPP

#include <cstdio>
#include <cstdlib>
#include <ST_util/math.hpp>

namespace ST {

    template <class T>
    class map {

        struct key_value{
            T value;
            size_t key = 0;
        };

    private:
        key_value* values = nullptr;

        size_t size = 0;
        size_t fill_amount = 0;

    public:

        ~map(){
            free(values);
            values = nullptr;
        }

        size_t get_size() {
            return size;
        }

        void reset(){
            free(values);
            values = nullptr;
            size = 0;
            fill_amount = 0;
        }

        void reset(size_t size){
            this->size = size;
            free(values);
            values = nullptr;
            values = static_cast<key_value*>(malloc(size * sizeof(key_value)));
        }

        bool exists(size_t key){
            if(size > 0){
                size_t result = key % (size-1);
                if(values[result].key == key){
                    return true;
                }else{
                    size_t counter = 0;
                    while(values[result].key != key){
                        result += 1;
                        if(result == size){
                            result = 0;
                        }
                        ++counter;
                        if(counter == size){
                            return false;
                        }
                    }
                    return true;
                }
            }else{
                return false;
            }
        }

        bool is_empty(){
            return fill_amount == 0;
        }

        void add(size_t key, T value){
            if(size == 0){
                size = 32;
                values = static_cast<key_value*>(malloc(size * sizeof(key_value)));
                for(size_t i = 0; i < size; i++){
                    values[i] = key_value();
                }
            }
            size_t result = key % (size-1);

            if(values[result].key == 0){
                values[result].key = key;
                values[result].value = value;
            }else{
                result += 1;
                while(values[result].key != 0){
                    result += 1;
                    if(result == size){
                        result = 0;
                    }
                }
                values[result].key = key;
                values[result].value = value;
            }

            //adjust size if needed
            ++fill_amount;
            if(fill_amount > size / 2){
                auto temp = static_cast<key_value*>(realloc(values, (size * 2)*(sizeof(key_value))));
                values = temp;
                for(size_t i = size; i < size*2; i++){
                    values[i] = key_value();
                }
                size = size*2;
            }
        }

        T get(size_t key){
            if(size > 0){
                size_t result = key % (size-1);
                if(values[result].key == key){
                    return values[result].value;
                }else{
                    size_t counter = 0;
                    while(values[result].key != key){
                        result += 1;
                        if(result == size){
                            result = 0;
                        }
                        ++counter;
                        if(counter == size){
                            return nullptr;
                        }
                    }
                    return values[result].value;
                }
            }else{
                return nullptr;
            }

        }
    };

}

#endif //SLAVICTALES_MAP_HPP
