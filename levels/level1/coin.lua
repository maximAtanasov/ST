coin = object:new()
coin.texture = "textures/coin.png"
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
    if self:overObject(player1) then
        playSound("sound/coin.wav")
        self:setVisible(false)
        self:setCollision(0,0,0,0)
    end
end
