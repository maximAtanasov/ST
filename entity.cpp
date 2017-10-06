#include "entity.hpp"
#include "physics.hpp"

//default constructor
entity::entity(int id){

    //general
    ID = id;
    x = 0;
    y = 0;
    velocityX = 0;
    velocityY = 0;

    //texture
    texture = 0;
    texW = 0;
    texH = 0;
    visibility = 1;
    isStatic = false;

    //physics
    colX = 0;
    colY = 0;
    mass = 0;
    offsetX = 0;
    offsetY = 0;
    affectedByPhysics = false;

    //animation
    animation = 0;
    spriteNum = 1;
    animationNum = 0;
}

//GENERAL FUNCTIONS//

unsigned int entity::getID(void){
    return ID;
}

int entity::getX(void){
    return x;
}

int entity::getY(void){
    return y;
}

int entity::setX(int X){
    int currentX = x;
    x = X;
    updateCollisionBox(x, y);
    if(affectedByPhysics)
        if(checkCollision(ID) == 0){//if there is a collision, don't modify x
            x = currentX;
            updateCollisionBox(x, y);
            return 0;
        }
    return 1;
}

int entity::setY(int Y){
    int currentY = y;
    y = Y;
    updateCollisionBox(x, y);
    if(affectedByPhysics)
        if(checkCollision(ID) == 0){//if there is a collision, don't modify y
            y = currentY;
            updateCollisionBox(x, y);
            return 0;
        }
    return 1;
}

void entity::setStatic(bool arg){
    isStatic = arg;
}

int entity::getXOffset(void){
    int Xoffset;
    if(isStatic)
        Xoffset = 0;
    else
        Xoffset = camera.x;
    return Xoffset;
}

int entity::getYOffset(void){
    int Yoffset;
    if(isStatic)
        Yoffset = 0;
    else
        Yoffset = camera.y;
    return Yoffset;
}

void entity::setVelocityX(int arg){
    velocityX = arg;
}

void entity::setVelocityY(int arg){
    velocityY = arg;
}

int entity::getVelocityX(void){
    return velocityX;
}

int entity::getVelocityY(void){
    return velocityY;
}

void entity::drawCoordinates(void){
    int Xoffset, Yoffset;
    if(isStatic){
        Xoffset = 0;
        Yoffset = 0;
    }else{
        Xoffset = camera.x;
        Yoffset = camera.y;
    }
    std::string tempX = "x: " + std::to_string(x);
    std::string tempY = "y: " + std::to_string(y);
    SDL_Colour colour_text = {255, 255, 0, 255};
    gRenderer.draw_text("fonts/OpenSans-Regular.ttf", tempX, x - Xoffset, y - Yoffset - texH, colour_text, 25, D_RENDERTEXT_FAST);
    gRenderer.draw_text("fonts/OpenSans-Regular.ttf", tempY, x - Xoffset, y - Yoffset - texH + 30, colour_text, 25, D_RENDERTEXT_FAST);
}

//TEXTURE FUNCTIONS//

int entity::getTexW(void){
    return texW;
}

int entity::getTexH(void){
    return texH;
}

void entity::setTexW(int width){
    texW = width;
}

void entity::setTexH(int height){
    texH = height;
}

int entity::getTexture(void){
    return texture;
}

void entity::setVisible(bool arg){
    visibility = arg;
}

void entity::setTexture(std::string arg){
    std::hash<std::string> hash_f;
    texture = hash_f(arg);
}

void entity::draw(void){
    if(isStatic)
        gRenderer.draw_texture(texture, x, y);
    else
        gRenderer.draw_texture(texture, x - camera.x, y - camera.y);
}

bool entity::isVisible(void){
    return visibility;
}

//PHYSICS FUNTIONS//

int entity::getColX(void){
    return colX;
}

int entity::getColY(void){
    return colX;
}

void entity::setCollisionBox(int OffsetX, int OffsetY, int X, int Y){
    if(X != 0 && Y != 0){
        colX = X - 1;
        colY = -Y + 1;
        offsetX = OffsetX;
        offsetY = OffsetY;
    }else{
        colX = 0;
        colY = 0;
        setAffectedByPhysics(false);
    }
    updateCollisionBox(x, y);
}

void entity::updateCollisionBox(int x, int y){
    collision.left = x + offsetX;
    collision.right = x + colX + offsetX;
    collision.top = y + colY + offsetY;
    collision.bottom = y + offsetY;
}

int entity::getColYOffset(void){
    return offsetY;
}

void entity::setAffectedByPhysics(bool arg){
    affectedByPhysics = arg;
}

bool entity::isAffectedByPhysics(void){
    return affectedByPhysics;
}

bool entity::collides(entity other){
    return !((collision.bottom < other.collision.top) | (collision.top > other.collision.bottom)
    | (collision.left > other.collision.right) | (collision.right < other.collision.left));
}

void entity::drawCollisionBox(void){
    int Xoffset, Yoffset;
    if(isStatic){
        Xoffset = 0;
        Yoffset = 0;
    }else{
        Xoffset = camera.x;
        Yoffset = camera.y;
    }
    if(isAffectedByPhysics()){
        SDL_Colour colour = {240, 0, 0, 100};
        gRenderer.draw_rectangle_filled(x - Xoffset + offsetX, y - Yoffset + offsetY, colX, colY, colour);
    }
    else{
        SDL_Colour colour2 = {0, 0, 220, 100};
        gRenderer.draw_rectangle_filled(x - Xoffset + offsetX, y - Yoffset + offsetY, colX, colY, colour2);
    }
}

void entity::setMass(int arg){
    mass = arg;
}

int entity::getMass(void){
    return mass;
}

//ANIMATION FUNCTIONS//

void entity::setAnimation(int arg){
    animation = arg;
}

int entity::getAnimation(void){
    return animation;
}

void entity::setAnimationNum(int arg){
    animationNum = arg;
}

void entity::setSpriteNum(int arg){
    spriteNum = arg;
}

void entity::drawAnimation(void){
    //here you can control the animation speed
    time = ticks >> 7; //ticks/128
    if(isStatic){
        gRenderer.draw_sprite(texture, x , y, m_mod(time, spriteNum), animation, animationNum, spriteNum);
    }else{
        gRenderer.draw_sprite(texture, x - camera.x , y - camera.y, m_mod(time, spriteNum), animation, animationNum, spriteNum);
    }
}
