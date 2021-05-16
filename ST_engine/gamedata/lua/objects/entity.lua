-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

--tells if the mouse is over an objects texture
function mouseOverTexture(object)
    return mouseOverTextureLua(object.ID)
end

--tells if the mouse is over an objects collisions box
function mouseOver(object)
    return mouseOverLua(object.ID)
end

function newEntityID()
    local temp = currentID
    currentID = currentID + 1
    return temp
end

--default constructor for all entities
function newEntity(self, x, y)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    if(x ~= nil and y ~= nil) then
        --general
        o.ID = newEntityID()
	    createEntity()
        setEntityX(o.ID, x)
        setEntityY(o.ID, y)

        --texture
        setEntityTexture(o.ID, o.texture)
        setEntityTexW(o.ID, o.texWidth)
        setEntityTexH(o.ID, o.texHeight)
        setEntityVisible(o.ID, o.isVisible)

        --animation
        setEntitySpriteNum(o.ID, o.spriteNum)
        setEntityAnimationNum(o.ID, o.animationNum)

        --physics
        setEntityAffectedByPhysics(o.ID, o.affectedByPhysics)
	    setEntityCollisionBox(o.ID, o.offsetColX, o.offsetColY, o.colX, o.colY)
    end
    return o
end

--generic game entity, from which all other entities inherit
entity = {
	texture = "";
    offsetColX = 0;
    offsetColY = 0;
	colX = 0;
	colY = 0;
    isVisible = false;
    affectedByPhysics = false;
    mass = 0;

    --animations
    spriteNum = 1;
    animationNum = 1;
}

--create a new instance of an entity (calls the constructor above)
function entity:new(x, y)
    return newEntity(self, x, y)
end

function entity:setAffectedByPhysics(arg)
    self.affectedByPhysics = arg
    setEntityAffectedByPhysics(self.ID, arg)
end

--set the x position of an entity
function entity:setX(x)
    setEntityX(self.ID, x)
end

--set the y position of an entity
function entity:setY(y)
    setEntityY(self.ID, y)
end

--set the x and y position of an entity
function entity:setXY(x, y)
	setEntityX(self.ID, x)
	setEntityY(self.ID, y)
end

--get the x coordinate of an entity
function entity:getX()
    return getEntityX(self.ID)
end

--get the y coordinate of an entity
function entity:getY()
    return getEntityY(self.ID)
end

--tells if an entity (without hard collision) is over another entity
function entity:overObject(other)
    return entityCollides(self.ID, other.ID)
end

--set the Texture width of an entity
function entity:setTexW(arg)
    setEntityTexW(self.ID, arg)
end

--set the Texture height of an entity
function entity:setTexH(arg)
    setEntityTexH(self.ID, arg)
end

function entity:setTextureScale(x, y)
    self.textureScaleX = x;
    self.textureScaleY = y;
    setEntityTextureScale(self.ID, x, y)
end

--get the Texture width of an entity
function entity:getTexW()
    return getEntityTexW(self.ID)
end

--get the Texture height of an entity
function entity:getTexH()
    return getEntityTexH(self.ID)
end

--set the horizontal velocity of an entity
function entity:setVelocityX(arg)
    setEntityVelocityX(self.ID, arg)
end

--set the vertical velocity of an entity
function entity:setVelocityY(arg)
    setEntityVelocityY(self.ID, arg)
end

--get the horizontal velocity of an entity
function entity:getVelocityX()
    return getEntityVelocityX(self.ID)
end

--get the vertical velocity of an entity
function entity:getVelocityY()
    return getEntityVelocityY(self.ID)
end

--set the collision box of an entity
function entity:setCollision(offsetX, offsetY, width, height)
    self.colX = width
    self.colY = height
    self.offsetColX = offsetX;
    self.offsetColY = offsetY;
    setEntityCollisionBox(self.ID, offsetX, offsetY, width, height)
end

--scale the collision box of an entity`
function entity:setCollisionScale(x, y)
    setEntityCollisionBox(self.ID, 0, 0, x*self.colX, y*self.colY)
end

--get the Width of the collision box of an entity
function entity:getColX()
    return getEntityColX(self.ID)
end

--get the Height of the collision box of an entity
function entity:getColY()
    return getEntityColY(self.ID)
end

--set the Visibility of an entity
function entity:setVisible(arg)
    self.isVisible = arg;
    setEntityVisible(self.ID, arg)
end

--set weather an entity is static (always drawn at the same coordinates, regardless of camera position)
function entity:setStatic(arg)
    setEntityStatic(self.ID, arg)
end

--set the Texture of an entity (filepath)
function entity:setTexture(arg)
    self.texture = arg
    setEntityTexture(self.ID, arg)
end

--play the corresponding animation of an entity (the corresponding index in the spritesheet)
function entity:playAnimation(arg)
    setEntityAnimation(self.ID, arg)
end

--delete entity
function entity:delete()
    setEntityVisible(self.ID, false)
    self:setCollision(0, 0, 0, 0)
    setEntityActive(self.ID, false)
    self = nil
end
