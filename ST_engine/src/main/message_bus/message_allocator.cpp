#include <SDL2/SDL_mutex.h>
#include <memory>
#include <message_bus/message_allocator.hpp>
#include <message_bus/message.hpp>
#include <message_bus/message_types.hpp>

//message_allocator implementation===============================================

message_allocator::message_allocator(){
    access_mutex = SDL_CreateMutex();
    pointer = 0;
    memory = (message*)malloc(sizeof(message)*memory_size);
    for(int i = 0; i < memory_size; i++){
        allocated[i] = false; //mark all memory as free
    }
}

message* message_allocator::allocate_message(msg_type name, std::shared_ptr<void> data){
    SDL_LockMutex(access_mutex);
    int i = 0;
    //find the next free spot in memory
    while(allocated[pointer] && i < memory_size){
        pointer++;
        i++;
        if(pointer > memory_size-1){
            pointer = 0;
        }
    }
    if(i < memory_size){ //if it exist, allocate with placement new in that memory block and pass the pointer as an id (used later for deallocation)
        allocated[pointer] = true;
        auto temp = new (memory+pointer) message(name, data, pointer);
        SDL_UnlockMutex(access_mutex);
        return temp;
    }
    else{//if there is no free memory, allocate it with malloc and pass -1 as an id so we know to free() it later
        SDL_UnlockMutex(access_mutex);
        return new (malloc(sizeof(message))) message(name, data, -1);
    }
}


void message_allocator::deallocate(int id, message* arg){
    if(id != -1){//with the help of the id we can mark the unused memory as free in our array
        SDL_LockMutex(access_mutex);
        allocated[id] = false;
        SDL_UnlockMutex(access_mutex);
    }
    else{//if id == -1 this means the memory was allocated with malloc on the go and needs to be freed with delete
        free(arg);
    }
}

message_allocator::~message_allocator(){
    SDL_DestroyMutex(access_mutex);
    free(memory);
}