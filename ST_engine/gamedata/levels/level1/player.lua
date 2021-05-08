-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

player = entity:new()
player.texture = "alien_blue.png"
player.texWidth = 128
player.texHeight = 256
player.offsetColX = 20
player.offsetColY = 0
player.colX = 88
player.colY = 155
player.idleRightAnim = 1
player.idleLeftAnim = 7
player.leftAnim = 5
player.rightAnim = 2
player.jumpLeft = 6
player.jumpRight = 3
player.lastDirection = 0
player.jumpC = 0
player.isVisible = true
player.prevY = 0
player.affectedByPhysics = true
player.speed = 15;
player.animationNum = 7
player.spriteNum = 2

player.width = 128;
player.height = 200;
player.textureScaleX = 1;
player.textureScaleY = 1;
player.speedSound = false

player.lives = 3

function player:update()
    if(self.prevY ~= self:getY()) then
        if(self.lastDirection == 0) then
            self:playAnimation(self.jumpRight)
        else
            self:playAnimation(self.jumpLeft)
        end
    end

    if(keyPressed("DASH")) then
        self.speed = 36;
        if self.speedSound == false then
            playSound("speed.wav", 20, 0)
            self.speedSound = true
            local velocity = self:getVelocityX()
            if self.lastDirection == 0 then
                self:setVelocityX(velocity + 66)
            else
                self:setVelocityX(velocity - 66)
            end
            return
        end
    else
        self.speedSound = false
        self.speed = 15;
    end

    leftStick = leftStickHorizontal()
    if leftStick < -2000 and leftStick > -15000 then
       self:moveLeft(5)
    elseif leftStick < -15000 then
        self:moveLeft(self.speed)
    elseif leftStick > 2000 and leftStick < 15000 then
        self:moveRight(5)
    elseif leftStick > 15000 then
        self:moveRight(self.speed)
    elseif keyHeld("LEFT") then
        self:moveLeft(self.speed)
    elseif keyHeld("RIGHT") then
        self:moveRight(self.speed)
    else
        if self.prevY == self:getY() then
            self:idle()
        end
    end
    if keyPressed("JUMP") then
        self:jump()
    end

    self.prevY = self:getY()
end

function player:jump()
    if self.prevY == self:getY() then
        playSound("phaseJump1.ogg", 50, 0)
        self:setVelocityY(-47)
    end

end

function player:moveRight(speed)
    self.lastDirection = 0;
    if self:getVelocityX() < 60 then
        self:setVelocityX(speed)
    end
    if self.prevY == self:getY() then
        self:playAnimation(self.rightAnim)
    end
end

function player:moveLeft(speed)
    self.lastDirection = 1;
    if self:getVelocityX() > -60 then
        self:setVelocityX(-speed)
    end
    if self.prevY == self:getY() then
        self:playAnimation(self.leftAnim)
    end
end

function player:idle()
    if self.lastDirection == 0 then
        self:playAnimation(self.idleRightAnim)
    else
        self:playAnimation(self.idleLeftAnim)
    end
end