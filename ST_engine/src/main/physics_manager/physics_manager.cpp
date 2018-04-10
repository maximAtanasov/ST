#include <physics_manager/physics_manager.hpp>

int physics_manager::initialize(message_bus* msg_bus, task_manager* tsk_mngr){
    gMessage_bus = msg_bus;
    gTask_manager = tsk_mngr;
    msg_sub = new subscriber();
    gMessage_bus->subscribe(SET_GRAVITY, msg_sub);
    gMessage_bus->subscribe(SET_FRICTION, msg_sub);
	gMessage_bus->subscribe(SET_FLOOR, msg_sub);
	gMessage_bus->subscribe(PAUSE_PHYSICS, msg_sub);
	gMessage_bus->subscribe(UNPAUSE_PHYSICS, msg_sub);
    gravity = 0;
    friction = 3;
    level_floor = 0;
    return 0;
}

void physics_manager::process_horizontal() {
    for(auto& i : *entities) {
        //handle horizontal velocity
        if (i.is_affected_by_physics()) {
            if (i.get_velocity_x() > 0) {
                for (int j = 0; j < i.get_velocity_x(); j++) {
                    if (entity_set_x(i.get_x() + 1, i.get_ID(), entities) == 0) {
                        break;
                    }
                }
                for (int j = 0; j < friction && i.get_velocity_x() > 0; j++) {
                    (i.set_velocity_x(i.get_velocity_x() - 1));
                }
            } else if (i.get_velocity_x() < 0) {
                for (int j = 0; j > i.get_velocity_x(); j--) {
                    if (entity_set_x(i.get_x() - 1, i.get_ID(), entities) == 0) {
                        break;
                    }
                }
                for (int j = 0; j < friction && i.get_velocity_x() < 0; j++) {
                    (i.set_velocity_x(i.get_velocity_x() + 1));
                }
            }
        }
    }
}

void physics_manager::process_vertical() {
    for(auto& i : *entities) {
        if (i.is_affected_by_physics()) {
            //handle vertical velocity
            const int objectVelocity = i.get_velocity_y() + gravity;
            if (objectVelocity < 0) {
                for (int j = 0; j > objectVelocity; j--) {
                    if (entity_set_y(i.get_y() - 1, i.get_ID(), entities) == 0) {
                        break;
                    }
                }
            } else if (objectVelocity > 0) {
                for (int j = 0; j < objectVelocity; j++) {
                    if (i.get_y() + i.get_col_y_offset() < level_floor) {
                        if (entity_set_y(i.get_y() + 1, i.get_ID(), entities) == 0) {
                            break;
                        }
                    }
                }
            }
            //decrease velocity of objects (apply gravity really)
            int realVelocity = objectVelocity - gravity;
            if (realVelocity < 0) {
                i.set_velocity_y(realVelocity + 2);
            }
        }
    }
}

void physics_manager::update(level_data* data){
    handle_messages();
	if(physics_paused){
		return;
	}
    entities = &data->entities;
    process_horizontal();
    process_vertical();
}

void physics_manager::handle_messages(){
    message* temp = msg_sub->get_next_message();
    while(temp != nullptr){
        if(temp->msg_name == SET_GRAVITY){
            gravity = *(int*)temp->get_data();
        }else if(temp->msg_name == SET_FLOOR){
            level_floor = *(int*)temp->get_data();
        }else if(temp->msg_name == SET_FRICTION){
            friction = *(int*)temp->get_data();
        }else if(temp->msg_name == PAUSE_PHYSICS){
            physics_paused = true;
        }else if(temp->msg_name == UNPAUSE_PHYSICS){
            physics_paused = false;
        }
        destroy_msg(temp);
        temp = msg_sub->get_next_message();
    }
}

int physics_manager::entity_set_x(int X, unsigned int ID, std::vector<entity>* entities){
    int currentX = entities->at(ID).get_x();
    entities->at(ID).set_x(X);
    if(entities->at(ID).is_affected_by_physics())
        if(check_collision(ID, entities) == 0){//if there is a collision, don't modify x
            entities->at(ID).set_x(currentX);
            return 0;
        }
    return 1;
}

int physics_manager::entity_set_y(int Y, unsigned int ID, std::vector<entity>* entities){
    int currentY = entities->at(ID).get_y();
    entities->at(ID).set_y(Y);
    if(entities->at(ID).is_affected_by_physics()){
        if(check_collision(ID, entities) == 0){//if there is a collision, don't modify y
            entities->at(ID).set_y(currentY);
            return 0;
        }
	}
    return 1;
}


int physics_manager::check_collision(unsigned int ID, std::vector<entity>* entities){
    for(unsigned int i = 0; i < entities->size(); i++){
        entity* temp = &entities->at(i);
        if(temp->is_affected_by_physics()){
            if(ID == i){
                continue;
            }
            if(temp->collides(entities->at(ID))){
                return 0;
            }
        }
    }
    return -1;
}


void physics_manager::close(){
    delete msg_sub;
}
