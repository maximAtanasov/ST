crystal_green = object:new()

crystal_green.texture = "crystal_green.png"
crystal_green.texWidth = 64
crystal_green.texHeight = 64
crystal_green.colX = 64
crystal_green.colY = 64
crystal_green.isVisible = true
crystal_green.spriteNum = 8
crystal_green.affectedByPhysics = false
crystal_green.animationNum = 1

function crystal_green:spin()
    self:playAnimation(1)
end

function crystal_green:update()
	if self.isVisible == true then
		if self:overObject(player1) then
		    playSound("coin.wav", 100, 0)
		    self:setVisible(false)
		    self:setCollision(0,0,0,0)
		end
	end
end
