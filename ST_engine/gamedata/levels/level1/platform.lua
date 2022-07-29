-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

PLATFORM_TYPE = {
    GRASS = "platform_grass.png",
    STONE = "platform_stone.png"
}

platform = entity:new()
platform.texture = PLATFORM_TYPE.STONE
platform.texWidth = 512
platform.texHeight = 128
platform.colX = 512
platform.colY = 128
platform.isVisible = true
platform.affectedByPhysics = true
platform.soundPlayed = false
platform.soundTrigger = nil
platform.shadow = entity:new()
platform.shadow.texture = "shadow.png"
platform.shadow.texWidth = 512
platform.shadow.texWidth = 512
platform.shadow.isVisible = true
platform.shadow.affectedByPhysics = false

function platform:update()
    self:setVelocityY(-gravity)
    if self.soundTrigger:overObject(player1) then
        player1:setVelocityY(-13)
        if self.soundPlayed == false then
           -- playSound("jump.wav", 128, 0)
            self.soundPlayed = true
        end
    else
        self.soundPlayed = false
    end
end

function platform:new(x, y)
    self = newEntity(self, x, y)
    self.soundTrigger = trigger:new(x, y+2, self.colX, 3)
    --self.shadow = newEntity(self.shadow, x, y + 512*2)
    self.shadow:setTextureScale(1, 2)
    return self;
end
