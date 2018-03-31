coin = object:new()
coin.texture = "coin.png"
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
