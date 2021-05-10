-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

coin = entity:new()
coin.texture = "coin.png"
coin.texWidth = 64
coin.texHeight = 64
coin.colX = 50;
coin.colY = 50;
coin.offsetColX = 7
coin.offsetColY = -7;
coin.isVisible = true
coin.affectedByPhysics = false

function coin:spin()
    self:playAnimation(1)
end

function coin:update()
    if self.isVisible == true then
		if self:overObject(player1) then
		    playSound("coin.wav", 70, 0) --string value is hashed away in preprocessing
			coinsCollected = coinsCollected + 1;
		   	self:delete()
		end
	end
end
