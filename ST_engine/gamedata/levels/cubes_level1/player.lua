-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

player = entity:new()
player.texture = "cube_eye.png"
player.texWidth = 150
player.texHeight = 150
player.offsetColX = 0
player.offsetColY = 0
player.colX = 150
player.colY = 150
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
player.speed = 5;

player.width = 150;
player.height = 150;
player.textureScaleX = 1;
player.textureScaleY = 1;
player.speedSound = false

player.lives = 3

function player:update()
    if(leftTrigger() > 20000) then
        if(self.speed > 5) then
            self.speed = 26;
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
        end
    else
        if(self.speed > 5) then
            if self.speedSound == true then
                self.speedSound = false
            end
            self.speed = 15;
        end
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
        self:idle()
    end
    if keyPressed("JUMP") then
        self:jump()
    end


    --handle jumping
    if self:getVelocityY() < 0 then
        if self.lastDirection == 1 then
           -- self:playAnimation(self.jumpLeft)
        elseif self.lastDirection == 0 then
            --self:playAnimation(self.jumpRight)
        end
    end
    self.prevY = self:getY()

end

function player:jump()
    if self.prevY == self:getY() then
        self:setVelocityY(-47)
    end
end

function player:moveRight(speed)
    self.lastDirection = 0;
    if self:getVelocityX() < 60 then
        self:setVelocityX(speed)
    end
    self:setTexture("cube_eye.png")
end

function player:moveLeft(speed)
    if self:getVelocityX() > -60 then
        self:setVelocityX(-speed)
    end
    self.lastDirection = 1;
    self:setTexture("cube_eye_left.png")
end

function player:idle()
    if self.lastDirection == 0 then
      --  self:playAnimation(self.idleRightAnim)
    else
    --    self:playAnimation(self.idleLeftAnim)
    end
end