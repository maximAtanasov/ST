#ifndef MNGR_INTERFACE
#define MNGR_INTERFACE

#include "message_bus/message_bus.hpp"
#include "task_manager/task_manager.hpp"

class manager{
    private:
        virtual void handle_messages() = 0;
    public:
        virtual int initialize(message_bus*, task_manager*) = 0;
        virtual void update() = 0;
        virtual void close() = 0;
};

#endif
