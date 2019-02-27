-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

trigger = entity:new()
trigger.texture = "coin.webp"
trigger.texWidth = 65
trigger.texHeight = 65
trigger.colX = 63;
trigger.colY = 63;
trigger.isVisible = true
trigger.animationNum = 1;
trigger.spriteNum = 8
trigger.affectedByPhysics = false

function trigger:update()
    if self.isVisible == true then
        if self:overObject(player1) then
            --playSound("coin.wav", 70, 0)
           -- self:delete()
        end
    end
end
