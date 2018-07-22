-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com

coin = entity:new()
coin.texture = "coin.webp"
coin.texWidth = 63
coin.texHeight = 63
coin.colX = 63;
coin.colY = 63;
coin.isVisible = true
coin.animationNum = 1;
coin.spriteNum = 8
coin.affectedByPhysics = false

function coin:spin()
    self:playAnimation(1)
end

function coin:update()
    if self.isVisible == true then
		if self:overObject(player1) then
		    playSound("coin.wav", 70, 0)
		   	self:delete()
		end
	end
end
