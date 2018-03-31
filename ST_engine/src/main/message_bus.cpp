#include "message_bus.hpp"

static message_allocator* msg_memory;

//allocator and constructor for messages
message* make_msg(msg_type name, const std::shared_ptr<void>& data){
    return msg_memory->allocate_message(name, data);
}

void destroy_msg(message* msg){
    msg_memory->deallocate(msg->get_id(), msg);
}

//message_bus implementation=====================================================

int message_bus::initialize(){
    //initialize the message allocator
    msg_memory = new message_allocator();
    return 0;
}

//send a message to all subscribers of that message type
//locks aren't really needed here as there won't be any new subscribers in the middle of the game
//(if you do want to have subsystems subscribe at random times you should definitely add locks)
void message_bus::send_msg(message* arg){
    int name = arg->msg_name;
    std::vector<subscriber*>* temp = &subscribers[name];
    if(temp->size() == 1){
        temp->at(0)->push_message(arg);
    }
    else{
        for(auto i : *temp){
            i->push_message(arg->make_copy()); //yes all queues are thread-safe so this is fine
        }
        destroy_msg(arg);
    }
}

//subscribe to a message type -adds the subscriber object to the list of subscribers for the given message type
void message_bus::subscribe(msg_type msg, subscriber* sub){
    int temp = msg;
    subscribers[temp].push_back(sub);
}

void message_bus::close(){
    delete msg_memory;
}


//message implementation=========================================================

//constructor - only ever used by the message_allocator class
message::message(msg_type name, const std::shared_ptr<void>& data, int id){
    msg_name = name;
    this->data = data;
    this->id = id;
}

void* message::get_data(){
    return this->data.get();
}

int message::get_id(){
    return id;
}

message* message::make_copy(){
    return msg_memory->allocate_message(this->msg_name, this->data);
}

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

//subscriber implementation

message* subscriber::get_next_message(){
    message* new_message;
    if(queue.try_dequeue(new_message)){
        return new_message;
    }else {
        return nullptr;
    }
}

//and again, the queues are thread-safe so no locks are needed
void subscriber::push_message(message* arg){
    queue.enqueue(arg);
}
