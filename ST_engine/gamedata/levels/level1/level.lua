--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

use("grass.lua")
use("tree1.lua")
use("plant1.lua")
use("player.lua")
use("house1.lua")
use("house2.lua")
use("barrel1.lua")
use("barrel2.lua")
use("crystal_green.lua")
use("crate.lua")
use("coin.lua")
use("wolf.lua")
use("torch.lua")

pause = 0

stopAllSounds()
setLevelSize(7000, 3000)
floor = 1050
gravity = 18
initialFade = 255
darkness = 195
setGravity(gravity)
setLevelFloor(1050)
setDarkness(darkness)
hideMouseCursor()

--Load the appropriate language configuration
if language == "russian" then
    use("language_russian.lua")
elseif language == "english" then
    use("language_english.lua")
end

setBackground("fortress.png")
setOverlay("rain.png", 17);
playMusic("Shades.ogg", 100, -1)
playSound("rain.wav", 50, -1)
--Create and initialize entities

plant1_2 = plant1:new(100, floor)
plant1_1 = plant1:new(150, floor)
plant1_3 = plant1:new(200, floor)

tree1_1 = tree1:new(725, floor)

house1_1 = house1:new(1700, floor)
house2_1 = house2:new(2500, floor)
house2_2 = house2:new(5000, floor)

torch1 = torch:new(1900, 950)
torch1:play_animation()

torch2 = torch:new(2780, 950)
torch2:play_animation()

torch2 = torch:new(5280, 950)
torch2:play_animation()

tree1_2 = tree1:new(1040, floor)
tree1_3 = tree1:new(2250, floor)
tree1_4 = tree1:new(2900, floor)

tree1_5 = tree1:new(4040, floor)
tree1_6 = tree1:new(6250, floor)
tree1_7 = tree1:new(8900, floor)

barrel1_1 = barrel1:new(2450, floor)
barrel2_1 = barrel2:new(3200, floor)

player1 = player:new(10, floor)
player1:idle()

--grass
crate1 = crate:new(400, floor)
crate2 = crate:new(3400, floor)
crate3 = crate:new(460, floor-150)
crate4 = crate:new(800, floor)
crate5 = crate:new(900, floor-150)
crate6 = crate:new(1000, floor)
crate7 = crate:new(2000, floor)

wolf1 = wolf:new(2500, 1000)
wolfTrigger1 = wolfTrigger:new(2500, 1000)
wolf1:idle()

grass1 = grass:new(0, 1081)
grass2 = grass:new(400, 1081)
grass3 = grass:new(800, 1081)
grass4 = grass:new(1200, 1081)
grass5 = grass:new(1600, 1081)
grass6 = grass:new(2000, 1081)
grass7 = grass:new(2400, 1081)
grass8 = grass:new(2800, 1081)
grass9 = grass:new(3200, 1081)
grass10 = grass:new(3600, 1081)
grass11 = grass:new(4000, 1081)
grass12 = grass:new(4400, 1081)
grass13 = grass:new(4800, 1081)
grass14 = grass:new(5200, 1081)
grass15 = grass:new(5600, 1081)
grass16 = grass:new(6000, 1081)
grass17 = grass:new(6400, 1081)
grass18 = grass:new(6800, 1081)
grass19 = grass:new(7200, 1081)
grass20 = grass:new(7600, 1081)
grass21 = grass:new(8000, 1081)
grass22 = grass:new(8400, 1081)
grass23 = grass:new(8800, 1081)
grass24 = grass:new(9200, 1081)
grass25 = grass:new(9600, 1081)
grass26 = grass:new(10000, 1081)

coin1 = coin:new(1400, 1000)
coin1:spin()

coin2 = coin:new(1470, 1000)
coin2:spin()

coin3 = coin:new(1540, 1000)
coin3:spin()

coin4 = coin:new(1610, 1000)
coin4:spin()

coin5 = coin:new(1680, 1000)
coin5:spin()

coin6 = coin:new(1750, 1000)
coin6:spin()

crystal_green1 = crystal_green:new(1850, 1000)
crystal_green1:spin()

coin7 = coin:new(3600, 1000)
coin7:spin()

coin8 = coin:new(3670, 1000)
coin8:spin()

coin9 = coin:new(3740, 1000)
coin9:spin()

coin10 = coin:new(3810, 1000)
coin10:spin()

coin11 = coin:new(3880, 1000)
coin11:spin()

coin12 = coin:new(3950, 1000)
coin12:spin()

light1 = light:new(1935, 850, 100, 85, 0)
light2 = light:new(2815, 850, 100, 85, 0)
light3 = light:new(5315, 850, 100, 85, 0)

--PAUSE MENU=========================================================================
pauseBG = entity:new(0, floor)
pauseBG:setTexture("fortress.png")
pauseBG:setVisible(false)
pauseBG:setStatic(true)
pauseBG:setY(1080)

--Buttons and their update functions
----@Key
all_buttons_key = "MOUSE1"

----@Audio
all_buttons_sound = "buttonSelected.wav"

--CONTINUE BUTTON
button_continue = button:new(50, 500, CONTINUE_TEXT, FONT, 80)
button_continue:setClickKey(all_buttons_key)
button_continue:setMarginX(474)
button_continue:setMarginY(92)
button_continue:hide()

function button_continue:onClick()
    unpauseGame()
end

function button_continue:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0;
    end
end

function button_continue:onNothing()
    self.soundPlayed = 1
end

--EXIT BUTTON
button_exit = button:new(50, 600, EXIT_TEXT, FONT, 80)
button_exit:setClickKey(all_buttons_key)
button_exit:setMarginX(224)
button_exit:setMarginY(85)
button_exit:hide()

function button_exit:onClick()
    endGame()
end

function button_exit:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0;
    end
end

function button_exit:onNothing()
    self.soundPlayed = 1
end

--FUNCTIONS FOR PAUSING THE GAME
function pauseGame()
    setOverlay(" ", 1)
    setDarkness(0)
    pausePhysics()
    setGravity(0)
    pauseBG:setVisible(true)
    button_continue:show()
    button_exit:show()
    showMouseCursor()
    pause = 1
end

function unpauseGame()
    setDarkness(darkness)
    setOverlay("rain.png", 17)
    unpausePhysics()
    setGravity(gravity)
    pauseBG:setVisible(false)
    button_continue:hide()
    button_exit:hide()
    hideMouseCursor()
    setBackground("fortress.png")
    pause = 0
end

--END PAUSE MENU
