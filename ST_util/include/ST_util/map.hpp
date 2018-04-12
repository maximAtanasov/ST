#ifndef SLAVICTALES_MASTER_MAP_HPP
#define SLAVICTALES_MASTER_MAP_HPP

#include <cstdio>
#include <cstdlib>
#include <ST_util/math.hpp>

namespace ST {

    template<class T> class map {
    private:
        T* values = nullptr;

        size_t size = 64;
        size_t fill_amount = 0;

    public:
        map(){
            values = static_cast<T*>(malloc(size * sizeof(T)));
            for(size_t i = 0; i < size; i++){
                values[i] = nullptr;
            }
        }

        ~map(){
            //free(values);
        }

        size_t get_size() {
            return size;
        }

        void reset(){
            //free(values);
            size = 64;
        }

        void add(size_t key, T value){
            values[ST::pos_mod(key, size)] = value;
            fill_amount += 1;
            if(fill_amount > size / 2){
                values = (T*)realloc(values, (size + size/2)*(sizeof(T)));
                for(size_t i = size; i < size + size/2; i++){
                    values[i] = nullptr;
                }
                size += size/2;
            }
        }

        T* get_values(){
            return values;
        }

        T get(size_t key){
            return values[ST::pos_mod(key, size)];
        }
    };

}

#endif //SLAVICTALES_MASTER_MAP_HPP
