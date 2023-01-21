-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

function makeSpikes(x, y)
    local spikes1 = entity:new(x, y)
    spikes1.texHeight = 128
    spikes1.texWidth = 128
    spikes1:setTexW(128)
    spikes1:setTexH(128)
    spikes1:setTexture("spikes.png")
    spikes1:setVisible(true)

    local trigger_death = trigger:new(x, y, 128, 50)
    trigger_death:onCollisionWith(player1, resetPlayerPosition)
    spikes1.update = function() trigger_death:update()  end
    return spikes1
end