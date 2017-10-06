#include "physics.hpp"
#include "level_manager.hpp"
#include "entity.hpp"

int gravity = 15;
int friction = 3;
int Levelfloor = 1080;

void applyPhysics(){
    for(std::vector<entity*>::iterator i = gLevel_manager.get_level_data()->entities.begin(); i != gLevel_manager.get_level_data()->entities.end(); i++){
        //handle horizontal velocity
        if((*i)->isAffectedByPhysics()){
            if((*i)->getVelocityX() > 0){
                for(int j = 0; j < (*i)->getVelocityX(); j++)
                    if((*i)->setX((*i)->getX()+1) == 0)
                        break;
                for(int j = 0; j < friction && (*i)->getVelocityX() > 0; j++)
                    ((*i)->setVelocityX((*i)->getVelocityX() - 1));
            }
            else if((*i)->getVelocityX() < 0){
                for(int j = 0; j > (*i)->getVelocityX(); j--)
                    if((*i)->setX((*i)->getX()-1) == 0)
                        break;
                for(int j = 0; j < friction && (*i)->getVelocityX() < 0; j++)
                    ((*i)->setVelocityX((*i)->getVelocityX() + 1));
            }
            //handle vertical velocity
            int objectVelocity = (*i)->getVelocityY() + gravity;
            if( objectVelocity < 0){
                for(int j = 0; j > objectVelocity ; j--){
                    if((*i)->setY((*i)->getY()-1) == 0)
                        break;
                }
            }
            else if(objectVelocity > 0){
                for(int j = 0; j < objectVelocity ; j++)
                    if((*i)->getY()+(*i)->getColYOffset() < Levelfloor)
                        if((*i)->setY((*i)->getY()+1) == 0)
                            break;
            }
        }
    }
}

int checkCollision(unsigned  ID){
    if(gLevel_manager.get_level_data()->entities.size() == 0)
        return -1;
    for(unsigned int i = 0; i < gLevel_manager.get_level_data()->entities.size(); i++){
        entity* temp = gLevel_manager.get_level_data()->entities[i];
        if(temp->isAffectedByPhysics()){
            if(ID == i)
                continue;
            if(temp->collides(*gLevel_manager.get_level_data()->entities[ID]))
                return 0;
        }
    }
    return -1;
}
