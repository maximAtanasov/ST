-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

wolf = entity:new()
wolf.texture = "wolf.webp"
wolf.texWidth = 300
wolf.texHeight = 150
wolf.colX = 250
wolf.colY = 120
wolf.animationNum = 6
wolf.spriteNum = 5
wolf.idleRightAnim = 3
wolf.idleLeftAnim = 6
wolf.moveRightAnim = 1
wolf.moveLeftAnim = 4
wolf.lastDirection = 1
wolf.isVisible = true
wolf.affectedByPhysics = true
wolf.mass = 0
wolf.idleCounter = 0;
wolf.prevX = 0;

wolfTrigger = entity:new()
wolfTrigger.affectedByPhysics = false;
wolfTrigger.colX = 252;
wolfTrigger.colY = 122;

function wolf:idle() 
    if self.lastDirection == 0 then
        self:playAnimation(self.idleRightAnim)
    else
        self:playAnimation(self.idleLeftAnim)
    end        
end

function wolf:update()
    if self.prevX == self:getX() then
        if self.lastDirection == 1 then
            self.lastDirection = 0
        elseif self.lastDirection == 0 then
            self.lastDirection = 1
        end
    end
    self.idleCounter = self.idleCounter + 1
    if self.idleCounter > 400 then
        self:walk()
    end
    self.prevX = self:getX()
    wolfTrigger1:setXY(self:getX() - 1, self:getY() - 1)
end
    

function wolf:walk()
    if self.lastDirection == 1 then
        self:setVelocityX(-5)
        self:playAnimation(self.moveLeftAnim)
    else
        self:setVelocityX(5)
        self:playAnimation(self.moveRightAnim)
    end       
end
