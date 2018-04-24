--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

player = object:new()
player.texture = "player2.png"
player.texWidth = 300
player.texHeight = 305
player.offsetColX = 50
player.offsetColY = 0
player.colX = 65
player.colY = 305
player.animationNum = 6
player.spriteNum = 6
player.idleRightAnim = 1
player.idleLeftAnim = 2
player.leftAnim = 3
player.rightAnim = 4
player.jumpLeft = 5
player.jumpRight = 6
player.lastDirection = 0
player.jumpC = 0
player.isVisible = true
player.prevY = 0
player.affectedByPhysics = true
player.mass = 10;

function player:update()
    if keyHeld("LEFT") then
        self:moveLeft()
    elseif keyHeld("RIGHT") then
        self:moveRight()
    else
        self:idle()
    end
    if keyPressed("JUMP") then
        self:jump()
    end

    --handle jumping
    if self:getVelocityY() < 0 then
        if self.lastDirection == 1 then
            self:playAnimation(self.jumpLeft)
        elseif self.lastDirection == 0 then
            self:playAnimation(self.jumpRight)
        end
    end
    self.prevY = self:getY()

    if self:overObject(wolfTrigger1) then
        self:setCollision(0, 0, 0, 0)
        playSound("sound/death.wav")
        startLevel("level1")
    end
end

function player:jump()
    if self.prevY == self:getY() then
        self:setVelocityY(-47)
    end
end

function player:moveRight()
    self.lastDirection = 0;
    self:setVelocityX(10)
    self:playAnimation(self.rightAnim)
end

function player:moveLeft()
    self:setVelocityX(-10)
    self.lastDirection = 1;
    self:playAnimation(self.leftAnim)
end

function player:idle()
    if self.lastDirection == 0 then
        self:playAnimation(self.idleRightAnim)
    else
        self:playAnimation(self.idleLeftAnim)
    end
end
