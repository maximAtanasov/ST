-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

platform = entity:new()
platform.texture = "obstacle_1.png"
platform.texWidth = 400
platform.texHeight = 150
platform.colX = 400
platform.colY = 132
platform.isVisible = true
platform.affectedByPhysics = true
platform.height = 0;
platform.soundPlayed = false
platform.soundTrigger = nil

function platform:update ()
    self:setY(self.height)
    if self.soundTrigger:overObject(player1) then
        if self.soundPlayed == false then
            playSound("jump.wav", 128, 0)
            self.soundPlayed = true
        end
    else
        self.soundPlayed = false
    end
end

function platform:new(x, y)
    self = newEntity(self, x, y)
    self.height = y;
    self.soundTrigger = trigger:new(x, y+2, self.colX, 3)
    return self;
end