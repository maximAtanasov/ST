--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com


currentID = 0 --MUST start at 0
currentVolume = 100;
math.randomseed( os.time() )

function newID()
    temp = currentID
    currentID = currentID + 1;
    return temp
end

--cleans up current level and start the specifed one
function startLevel(arg)
    startLevelLua(arg)
    currentID = 0
    error() --a dirty trick, but works
end

--tells if the mouse is over an object
function mouseOver(object)
    if getMouseX() < object:getTexW() + object:getX() and getMouseX() > object:getX() then
        if getMouseY() > object:getY() - object:getTexH() and getMouseY() < object:getY() then
            return true
        end
    end
    return false
end

--default constructor for all entities
function new(self, x, y)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    if(x ~= nil and y ~= nil) then
        --general
        o.ID = newID()
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

--genieric game object, from which all objects inherit
object = {
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

--create a new instance of an object (calls the constructor above)
function object:new(x, y)
    return new(self, x, y)
end

--set the x position of an object
function object:setX(x)
    setX(self.ID, x)
end

--set the y position of an object
function object:setY(y)
    setY(self.ID, y)
end

--set the x and y position of an object
function object:setXY(x, y)
	setX(self.ID, x)
	setY(self.ID, y)
end

--get the x corrdinate of an object
function object:getX()
    return getX(self.ID)
end

--get the y corrdinate of an object
function object:getY()
    return getY(self.ID)
end

--tells if an object (without hard collision) is over another object
function object:overObject(other)
    return entityCollides(self.ID, other.ID)
end

--set the Texture width of an object
function object:setTexW(arg)
    self.texWidth = arg;
    setTexW(self.ID, arg)
end

--set the Texture height of an object
function object:setTexH(arg)
    self.texHeight = arg;
    setTexH(self.ID, arg)
end

--get the Texture width of an object
function object:getTexW()
    return self.texWidth
end

--get the Texture height of an object
function object:getTexH()
    return self.texHeight
end

--set the horizontal velocity of an object
function object:setVelocityX(arg)
    setVelocityX(self.ID, arg)
end

--set the vertical velocity of an object
function object:setVelocityY(arg)
    setVelocityY(self.ID, arg)
end

--get the horizontal velocity of an object
function object:getVelocityX()
    return getVelocityX(self.ID)
end

--get the vertical velocity of an object
function object:getVelocityY()
    return getVelocityY(self.ID)
end

--set the mass of an object
function object:setMass(arg)
    self.mass = arg;
    setMass(self.ID, arg)
end

--get the mass of an object
function object:getMass()
    return getMass(self.ID)
end

--set the collision box of an object
function object:setCollision(offsetX, offsetY, width, height)
    self.colX = width
    self.colY = height
    setCollisionBox(self.ID, offsetX, offsetY, self.colX, self.colY)
end

--get the Width of the collision box of an object
function object:getColX()
    return getColX(self.ID)
end

--get the Height of the collision box of an object
function object:getColY()
    return getColY(self.ID)
end

--set the Visibility of an object
function object:setVisible(arg)
    self.isVisible = arg;
    setVisible(self.ID, arg)
end

--set weather an object is static (always drawn at the same corrdinates, regardless of camera position)
function object:setStatic(arg)
    setStatic(self.ID, arg)
end

--set the Texture of an object (filepath)
function object:setTexture(arg)
    self.texture = arg
    setTexture(self.ID, arg)
end

--play the corresponding animation of an object (the corresponding index in the spritesheet)
function object:playAnimation(arg)
    setAnimation(self.ID, arg)
end

--delete object
function object:delete()
    setVisible(self.ID, false)
    self:setCollision(0, 0, 0, 0)
    setActive(self.ID, false)
    self = nil
end

--TEXT

currentTextID = 0 --MUST start at 0

function newTextID()
    temp = currentTextID
    currentTextID = currentTextID + 1;
    return temp
end

textObject = {
	ID;
	x;
	y;
	text = " ";
	font = " ";
	fontSize = 40;
	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

function newTextObject(self, x, y, fontSize, font)
    o = {}
    setmetatable(o, self)
    self.__index = self
    if(x ~= nil and y ~= nil) then
        --general
        o.ID = newTextID()
		createTextObject(o.ID, x, y, o.text, font, fontSize);
    end
    return o
end

function textObject:new(x, y, fontSize, font)
	return newTextObject(self, x, y, fontSize, font)
end

function textObject:setText(text)
	setTextObjectText(self.ID, text)
end

function textObject:setFont(font)
	setTextObjectFont(self.ID, font)
end

function textObject:setFontSize(size)
	setTextObjectFontSize(self.ID, size)
end

function textObject:setTextColor(r, g, b, a)
	setTextObjectColor(self.ID, r, g, b, a)
end

function textObject:setTextVisible(arg)
    setTextObjectVisible(self.ID, arg)
end

--LIGHTING

light = {
    origin_x;
    origin_y;
    radius;
    intensity;
    brightness;
}

function newLight(origin_x, origin_y, radius, intensity, brightness)
    createLight(origin_x, origin_y, radius, intensity, brightness)
end
