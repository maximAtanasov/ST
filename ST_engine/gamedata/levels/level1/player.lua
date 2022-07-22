-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

PLAYER_TYPE = {
    BLUE = 0,
    PINK = 1,
    GREEN = 2,
    YELLOW = 3,
    BEIGE = 4
}

player = entity:new()
player.texWidth = 128
player.texHeight = 256
player.offsetColX = 20
player.offsetColY = 0
player.colX = 88
player.colY = 140
player.idleRightAnim = 1
player.idleLeftAnim = 6
player.leftAnim = 4
player.rightAnim = 2
player.jumpLeft = 5
player.jumpRight = 3
player.lastDirection = 0
player.jumpC = 0
player.isVisible = true
player.prevY = 0
player.affectedByPhysics = true
player.speed = 15;
player.animationNum = 8
player.spriteNum = 2
player.playerType = PLAYER_TYPE.GREEN
player.textureScaleX = 1
player.textureScaleY = 1
player.speedSound = false
player.jumpVelocity = -47

player.lives = 3

function player:new(x, y)
    self = newEntity(self, x, y)
    self:setPlayerType(self.playerType)
    return self
end

function player:update()
    if(self.prevY ~= self:getY()) then
        if(self.lastDirection == 0) then
            self:playAnimation(self.jumpRight)
        else
            self:playAnimation(self.jumpLeft)
        end
    end

    if keyPressed("DASH") then
        self.speed = 36;
        if self.speedSound == false then
            --playSound("speed.wav", 20, 0)
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
        --playSound("phaseJump1.ogg", 50, 0)
        self:setVelocityY(self.jumpVelocity)
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

function player:setPlayerType(playerType)
    if playerType == PLAYER_TYPE.BLUE then
        self:setCollision(20, 0, 88, 140)
        self.jumpVelocity = -47
        self.speed = 15
        self:setTexture("alien_blue.png")
    elseif playerType == PLAYER_TYPE.GREEN then
        self:setCollision(20, 0, 88, 140)
        self.jumpVelocity = -47
        self.speed = 15
        self:setTexture("alien_green.png")
    elseif playerType == PLAYER_TYPE.PINK then
        self:setCollision(20, 0, 88, 140)
        self.jumpVelocity = -47
        self.speed = 15
        self:setTexture("alien_pink.png")
    elseif playerType == PLAYER_TYPE.BEIGE then
        self:setCollision(20, 0, 88, 140)
        self.jumpVelocity = -55
        self.speed = 15
        self:setTexture("alien_beige.png")
    elseif playerType == PLAYER_TYPE.YELLOW then
        self:setCollision(20, 0, 88, 130)
        self.jumpVelocity = -44
        self.speed = 21
        self:setTexture("alien_yellow.png")
    end
end