-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

infoBlock = entity:new()
infoBlock.texture = "infoBlock.png"
infoBlock.texWidth = 128
infoBlock.texHeight = 128
infoBlock.colX = 128
infoBlock.colY = 128
infoBlock.isVisible = true
infoBlock.height = 0
infoBlock.affectedByPhysics = true
infoBlock.soundPlayed = false
infoBlock.soundTrigger = nil

function infoBlock:update()
    self:setY(self.height)
    if self.soundTrigger:overObject(player1) then
        player1:setVelocityY(-13)
        if self.soundPlayed == false then
            --playSound("jump.wav", 128, 0)
            self.soundPlayed = true
        end
    else
        self.soundPlayed = false
    end
end

--Must be created after the player in order to work correctly
function infoBlock:new(x, y)
    self.colY = 128 - getGravity()
    self = newEntity(self, x, y)
    self.height = y;
    self.soundTrigger = trigger:new(x, y+2, self.colX, 3)
    return self;
end
