card = entity:new()

card.value=0
card.texture="card.png"
card.texWidth = 120
card.texHeight = 758
card.isVisible = true
card.hover=false
card.currentHeight=0

card.goingRight = false
card.rightMotionCounter = 0
card.goingUp = false
card.play = false

function card:update()
    if keyHeld("MOUSE1") then
        if self.goingRight == true then
            self:goRight()
        elseif self.goingRight == false then
            self:goLeft()
        end
        if mouseOver(self) and self.hover == false then
            self:goUp()
        elseif mouseOver(self) == false then
            self:goDown()
        end
    else
        self:goDown()
        self:goLeft()
    end
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
    if self.currentHeight > baseCardHeight-400 then
        self.currentHeight = self.currentHeight - 30
        self:setY(self.currentHeight)
        self.goingUp = true
    else
        self.hover = true
    end
end