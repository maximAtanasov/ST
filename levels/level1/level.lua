use("grass.lua")
use("tree1.lua")
use("plant1.lua")
use("player.lua")
use("house1.lua")
use("house2.lua")
use("barrel1.lua")
use("barrel2.lua")
use("button.lua")
use("crystal_green.lua")
use("crate.lua")
use("coin.lua")
use("wolf.lua")

pause = 0;

setLevelSize(3000, 3000)
floor = 1081
gravity = 15
setGravity(gravity)
setLevelFloor(floor)

hideMouseCursor()

setBackground("textures/fortress.png")

--Create and initialize entities

plant1_2 = plant1:new(100, floor)

plant1_1 = plant1:new(150, floor)

plant1_3 = plant1:new(200, floor)

tree1_1 = tree1:new(725, floor)



house1_1 = house1:new(1700, floor)

house2_1 = house2:new(2500, floor)



tree1_2 = tree1:new(1040, floor)

tree1_3 = tree1:new(2250, floor)

tree1_4 = tree1:new(2900, floor)

barrel1_1 = barrel1:new(2450, floor)

barrel2_1 = barrel2:new(3200, floor)

player1 = player:new(10, floor)
player1:idle()

--grass
crate1 = crate:new(400, floor)
crate2 = crate:new(3400, floor)
crate3 = crate:new(460, floor-950)
crate4 = crate:new(800, floor)
crate5 = crate:new(900, floor-150)
crate6 = crate:new(1000, floor)
crate7 = crate:new(2000, floor)

wolf1 = wolf:new(2500, 1000);
wolfTrigger1 = wolfTrigger:new(2500, 1000);
wolf1:idle();

grass1 = grass:new(0, floor)
grass2 = grass:new(400, floor)
grass3 = grass:new(800, floor)
grass4 = grass:new(1200, floor)
grass5 = grass:new(1600, floor)
grass6 = grass:new(2000, floor)
grass7 = grass:new(2400, floor)
grass8 = grass:new(2800, floor)
grass9 = grass:new(3200, floor)
grass10 = grass:new(3600, floor)
grass11 = grass:new(4000, floor)
grass12 = grass:new(4400, floor)
grass13 = grass:new(4800, floor)

coin1 = coin:new(1400, 1000)
coin1:spin();

coin2 = coin:new(1470, 1000)
coin2:spin();

coin3 = coin:new(1540, 1000)
coin3:spin();

coin4 = coin:new(1610, 1000)
coin4:spin();

coin5 = coin:new(1680, 1000)
coin5:spin();

coin6 = coin:new(1750, 1000)
coin6:spin();

crystal_green1 = crystal_green:new(1850, 1000)
crystal_green1:spin();

coin7 = coin:new(3600, 1000)
coin7:spin();

coin8 = coin:new(3670, 1000)
coin8:spin();

coin9 = coin:new(3740, 1000)
coin9:spin();

coin10 = coin:new(3810, 1000)
coin10:spin();

coin11 = coin:new(3880, 1000)
coin11:spin();

coin12 = coin:new(3950, 1000)
coin12:spin();

--PAUSE MENU=========================================================================
pauseBG = object:new(0, floor)
pauseBG:setTexture("textures/fortress.png")
pauseBG:setVisible(false)
pauseBG:setStatic(true)
pauseBG:setY(1080)

--Buttons and their update functions



--CONTINUE BUTTON
button_continue = button:new(50, 500)
button_continue:setTexture("textures/buttonContinue.png")
button_continue:setTexW(474)
button_continue:setTexH(92)
button_continue:setStatic(true)

function button_continue:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
        self:setTexture("textures/buttonContinueSelected.png")
        if keyPressed("MOUSE1") then
            unpauseGame()
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonContinue.png")
    end
end


--EXIT BUTTON
button_exit = button:new(50, 600)
button_exit :setTexture("textures/buttonExit.png")
button_exit:setTexW(224)
button_exit:setTexH(85)
button_exit:setStatic(true)

function button_exit:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
    self:setTexture("textures/buttonExitSelected.png")
        if keyPressed("MOUSE1") then
            endGame()
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonExit.png")
    end
end



--FUNCTIONS FOR PAUSING THE GAME
function pauseGame()
    setGravity(0)
    pauseBG:setVisible(true)
    button_continue:setVisible(true)
    button_exit:setVisible(true)
    showMouseCursor()
    pause = 1
end

function unpauseGame()
    setGravity(gravity)
    pauseBG:setVisible(false)
    button_continue:setVisible(false);
    button_exit:setVisible(false);
    hideMouseCursor()
    setBackground("textures/fortress.png")
    pause = 0
end

--END PAUSE MENU
