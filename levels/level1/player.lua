player = object:new()
player.texture = "textures/player2.png"
player.texWidth = 300
player.texHeight = 305
player.offsetColX = 50
player.offsetColY = 0
player.colX = 65
player.colY = 305
player.animationNum = 6
player.spriteNum = 6
player.idleRightAnim = 1
player.idleLeftAnim = 2
player.leftAnim = 3
player.rightAnim = 4
player.jumpLeft = 5
player.jumpRight = 6    
player.lastDirection = 0
player.jumpC = 0
player.isVisible = true
player.prevY = 0
player.affectedByPhysics = true
player.mass = 10;
 
function player:update()
    if keyHeld("LEFT") then
        self:moveLeft()
    elseif keyHeld("RIGHT") then
        self:moveRight()
    else
        self:idle()
    end
    if keyPressed("JUMP") then
        self:jump()
    end
    if keyHeld("PUSH") then
        self:setMass(20)
    else
        self:setMass(0)
    end
    --handle jumping
    if self:getVelocityY() < 0 then
        self:setVelocityY(self:getVelocityY() + 2)
        if self.lastDirection == 1 then
            self:playAnimation(self.jumpLeft)
        elseif self.lastDirection == 0 then
            self:playAnimation(self.jumpRight)
        end
    end
    self.prevY = self:getY()

    if self:overObject(wolfTrigger1) then
        self:setCollision(0, 0, 0, 0)        
        playSound("sound/death.wav")
        delay(1000)
        startLevel("level1")
    end
end

function player:jump()
    if self.prevY == self:getY() then
        self:setVelocityY(-44)
    end
end

function player:moveRight()
    self.lastDirection = 0;
    self:setVelocityX(10)
    self:playAnimation(self.rightAnim)
end

function player:moveLeft()
    self:setVelocityX(-10)
    self.lastDirection = 1;
    self:playAnimation(self.leftAnim)
end

function player:idle() 
    if self.lastDirection == 0 then
        self:playAnimation(self.idleRightAnim)
    else
        self:playAnimation(self.idleLeftAnim)
    end        
end




