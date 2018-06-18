--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

--tells if the mouse is over an object
function mouseOver(object)
    local mouseX = getMouseX()
    local mouseY = getMouseY()
    local objectX = getX(object.ID)
    local objectY = getY(object.ID)
    if mouseX < getTexW(object.ID) + objectX and mouseX > objectX then
        if mouseY > objectY - getTexH(object.ID) and mouseY < objectY then
            return true
        end
    end
    return false
end

function newEntityID()
    local temp = currentID
    currentID = currentID + 1;
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
	    createEntity(o.ID)
        setX(o.ID, x)
        setY(o.ID, y)

        --texture
        setTexture(o.ID, o.texture)
        setTexW(o.ID, o.texWidth)
        setTexH(o.ID, o.texHeight)
        setVisible(o.ID, o.isVisible)

        --animation
        setSpriteNum(o.ID, o.spriteNum)
        setAnimationNum(o.ID, o.animationNum)

        --physics
        setAffectedByPhysics(o.ID, o.affectedByPhysics)
	    setCollisionBox(o.ID, o.offsetColX, o.offsetColY, o.colX, o.colY)
        setMass(o.ID, o.mass)
    end
    return o
end

--genieric game entity, from which all other entities inherit
entity = {
	texture = "";
    offsetColX = 0;
    offsetColY = 0;
	colX = 0;
	colY = 0;
    isVisible = false;
    texWidth = 0;
    texHeight = 0;
    affectedByPhysics = false;
    mass = 0;

    --animations
    spriteNum = 1;
    animationNum = 0;
}

--create a new instance of an entity (calls the constructor above)
function entity:new(x, y)
    return newEntity(self, x, y)
end

--set the x position of an entity
function entity:setX(x)
    setX(self.ID, x)
end

--set the y position of an entity
function entity:setY(y)
    setY(self.ID, y)
end

--set the x and y position of an entity
function entity:setXY(x, y)
	setX(self.ID, x)
	setY(self.ID, y)
end

--get the x corrdinate of an entity
function entity:getX()
    return getX(self.ID)
end

--get the y corrdinate of an entity
function entity:getY()
    return getY(self.ID)
end

--tells if an entity (without hard collision) is over another entity
function entity:overObject(other)
    return entityCollides(self.ID, other.ID)
end

--set the Texture width of an entity
function entity:setTexW(arg)
    self.texWidth = arg;
    setTexW(self.ID, arg)
end

--set the Texture height of an entity
function entity:setTexH(arg)
    self.texHeight = arg;
    setTexH(self.ID, arg)
end

--get the Texture width of an entity
function entity:getTexW()
    return self.texWidth
end

--get the Texture height of an entity
function entity:getTexH()
    return self.texHeight
end

--set the horizontal velocity of an entity
function entity:setVelocityX(arg)
    setVelocityX(self.ID, arg)
end

--set the vertical velocity of an entity
function entity:setVelocityY(arg)
    setVelocityY(self.ID, arg)
end

--get the horizontal velocity of an entity
function entity:getVelocityX()
    return getVelocityX(self.ID)
end

--get the vertical velocity of an entity
function entity:getVelocityY()
    return getVelocityY(self.ID)
end

--set the mass of an entity
function entity:setMass(arg)
    self.mass = arg;
    setMass(self.ID, arg)
end

--get the mass of an entity
function entity:getMass()
    return getMass(self.ID)
end

--set the collision box of an entity
function entity:setCollision(offsetX, offsetY, width, height)
    self.colX = width
    self.colY = height
    setCollisionBox(self.ID, offsetX, offsetY, self.colX, self.colY)
end

--get the Width of the collision box of an entity
function entity:getColX()
    return getColX(self.ID)
end

--get the Height of the collision box of an entity
function entity:getColY()
    return getColY(self.ID)
end

--set the Visibility of an entity
function entity:setVisible(arg)
    self.isVisible = arg;
    setVisible(self.ID, arg)
end

--set weather an entity is static (always drawn at the same corrdinates, regardless of camera position)
function entity:setStatic(arg)
    setStatic(self.ID, arg)
end

--set the Texture of an entity (filepath)
function entity:setTexture(arg)
    self.texture = arg
    setTexture(self.ID, arg)
end

--play the corresponding animation of an entity (the corresponding index in the spritesheet)
function entity:playAnimation(arg)
    setAnimation(self.ID, arg)
end

--delete entity
function entity:delete()
    setVisible(self.ID, false)
    self:setCollision(0, 0, 0, 0)
    setActive(self.ID, false)
    self = nil
end
