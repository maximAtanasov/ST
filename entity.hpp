#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "defs.hpp"
#include "renderer.hpp"

extern Uint32 ticks;

typedef struct collisionData{
    int left;
    int right;
    int top;
    int bottom;
} collisionData;

class entity{
    private:
        //general
        unsigned int ID;
        int x;
        int y;
        int velocityX;
        int velocityY;

        //animation
        int time;
        int spriteNum;
        int animation;
        int animationNum;

        //texture
        int texture;
        int texW;
        int texH;
        bool visibility;
        bool isStatic; //does not move with camera

        //padding
        char padding1[2];

        //physics
        int colX;
        int colY;
        int mass;
        int offsetX;
        int offsetY;
        collisionData collision;
        bool affectedByPhysics;

        //padding
        char padding2[3];


    public:
        //general
        entity(int);
        int setX(int);
        int setY(int);
        int getX(void);
        int getY(void);
        unsigned int getID(void);
        void destroy(void);
        void setStatic(bool);
        int getXOffset(void);
        int getYOffset(void);
        void setVelocityX(int);
        void setVelocityY(int);
        int getVelocityX(void);
        int getVelocityY(void);
        void drawCoordinates(void);

        //texture
        void draw(void);
        int getTexW(void);
        int getTexH(void);
        void setTexW(int);
        void setTexH(int);
        void setVisible(bool);
        bool isVisible(void);
        int getTexture(void);
        void setTexture(std::string);
        inline bool is_onscreen(){
            if(!visibility)
                return false;
            if(isStatic)
                return true;
            if(x-camera.x + texW >= 0 && x-camera.x <= 1920
            && y-camera.y  > 0 && y-camera.y - texH <= 1080)
                return true;
            return false;

        }

        //physics
        int getColX(void);
        int getColY(void);
        int getColYOffset(void);
        bool collides(entity);
        void setCollisionBox(int, int, int, int);
        void updateCollisionBox(int, int);
        void setAffectedByPhysics(bool);
        bool isAffectedByPhysics(void);
        void drawCollisionBox(void);
        int getMass(void);
        void setMass(int);

        //animation
        void setAnimationNum(int);
        void setSpriteNum(int);
        void setAnimation(int);
        int getAnimation(void);
        void drawAnimation(void);
};

#endif
