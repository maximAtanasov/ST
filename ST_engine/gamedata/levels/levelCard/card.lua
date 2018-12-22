card = entity:new()

card.value=0
card.texture="kingHeart.png"
card.texWidth = 120
card.texHeight = 750
card.colX=120
card.colY=750
card.isVisible = true
card.hover=false
card.currentHeight=0
card.onField = false
card.goingRight = false
card.rightMotionCounter = 0
card.goingUp = false
card.play = false
card.scale = 1
card.affectedByPhysics = false

function card:update()
    if self.onField then
        self:deployToField()
        return
    end
    if keyHeld("MOUSE1") then
        if self.goingRight == true then
            self:goRight()
        elseif self.goingRight == false then
            self:goLeft()
        end
        if mouseOver(self) then
            if self.hover == false then
                self:goUp()
            elseif (keyPressed("SELECT")) then
                self:deployToField()
            end
        else
            self:goDown()
        end
    else
        self:goDown()
        self:goLeft()
    end
end

function card:deployToField()
    self.onField = true
    if self.currentHeight > 1300 then
        self.currentHeight = self.currentHeight - 53
        self:setY(self.currentHeight)
        self:setTextureScale(self.scale, self.scale)
        self:setCollisionScale(self.scale, self.scale)
        self.scale = self.scale-0.03
    end
    self.goingUp = false
    self.hover = false
end

function card:goRight()
    if(self.rightMotionCounter < 300) then
        self:setX(self:getX() + 20)
        self.rightMotionCounter = self.rightMotionCounter + 20
    end
end

function card:goLeft()
    self.goingRight = false
    if( self.rightMotionCounter >= 0) then
        self:setX(self:getX() - 20)
        self.rightMotionCounter = self.rightMotionCounter - 20
    end
end

function card:goDown()
    if self.currentHeight < baseCardHeight then
        self.currentHeight = self.currentHeight + 30
        self:setY(self.currentHeight)
        self.goingUp = false
    else
        self.goingUp = false
        self.hover = false
    end
end

function card:goUp()
    if self.currentHeight > baseCardHeight-200 then
        self.currentHeight = self.currentHeight - 20
        self:setY(self.currentHeight)
        self.goingUp = true
    else
        self.hover = true
    end
end
