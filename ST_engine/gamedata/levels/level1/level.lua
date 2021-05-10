-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

use("player.lua")
use("trigger.lua")
use("obstacle_1.lua")
use("platform.lua")
use("bar_hor.lua")
use("crate.lua")
use("coin.lua")
use("water.lua")
use("edge_water.lua")
use("decal.lua")
use("spikes.lua")

pause = 0

stopAllSounds()
unpausePhysics()
enableLighting(false)

setLevelSize(150000, 3000)
floor = 1081
gravity = 18

coinsCollected = 0;

setGravity(gravity)

setLevelFloor(floor)
setDarkness(0)
hideMouseCursor()

setBackground("bg2_1.png", 0, 0)
setBackground("bg2_2.png", 1, 3)
setBackground("bg2_3.png", 2, 4)
setBackground("bg2_4.png", 3, 3)

--playMusic("music5.ogg", 40, -1)
--playSound("rain.wav", 50, -1)
--Create and initialize entities

endLevel = entity:new(20500,floor-100)
endLevel:setVisible(true)
endLevel:setTexture("black.png")
endLevel:setTexW(50)
endLevel:setTexH(50)
endLevel:setTextureScale(3, 3)
endLevel:setCollision(150,0, 5, 30)
endLevel:setAffectedByPhysics(true)

function endLevel:update()
    if(player1:getX() > self:getX()) then
        self:setAffectedByPhysics(false)
    else
        self:setAffectedByPhysics(true)
    end
    if(player1:getX() == 20500 and player1:getY() == self:getY()) then
        startLevel("cubes/cubes_level2")
    end
end

levelBeginBlock = entity:new(-10, floor)
levelBeginBlock:setAffectedByPhysics(true)
levelBeginBlock:setCollision(0,0,10, 1080)


bar_ho1_1 = bar_hor:new(0, floor);
bar_ho1_2 = bar_hor:new(512, floor);
bar_ho1_3 = bar_hor:new(1024, floor);
bar_ho1_4 = bar_hor:new(1536, floor);
bar_ho1_5 = bar_hor:new(2048, floor);
bar_ho1_6 = bar_hor:new(2560, floor);
bar_ho1_7 = bar_hor:new(3072, floor);
bar_ho1_8 = bar_hor:new(3584, floor);
--bar_ho1_9 = bar_hor:new(6400, floor);

bar_ho1_10 = bar_hor:new(4096, floor);
bar_ho1_11 = bar_hor:new(4608, floor);
bar_ho1_12 = bar_hor:new(5120, floor);
bar_ho1_13 = bar_hor:new(5632, floor);
edge_water_1 = edge_water:new(6144, floor);
water_1 = water:new(6272, floor);
water_2 = water:new(6784, floor);
water_3 = water:new(7296, floor);
water_3 = water:new(7808, floor);

bar_hor:new(8000, floor):setTexture("ground_grass2.png");
bar_hor:new(8768, floor):setTexture("ground_grass3.png");

bar_ho1_14 = bar_hor:new(9280, floor);
bar_ho1_15 = bar_hor:new(9792, floor);
bar_ho1_16 = bar_hor:new(10304, floor);
bar_ho1_17 = bar_hor:new(10816, floor);
bar_ho1_18 = bar_hor:new(11328, floor);

crate1 = crate:new(500, floor-200);
crate2 = crate:new(540, floor-400);
crate3 = crate:new(800, floor-600);


obstacle_1_4 = platform:new(2000, floor-400);
obstacle_1_5 = platform:new(3000, floor-200);
obstacle_1_6 = platform:new(3500, floor-400);
obstacle_1_7 = platform:new(4050, floor-290);
crate4 = crate:new(5650, floor-300);

obstacle_1_10 = platform:new(6100, floor-300);

obstacle_1_10_dialogTrigger = trigger:new(6130, floor-260, 360, 60);

obstacle_1_11 = platform:new(6700, floor-300);
obstacle_1_12 = platform:new(7250, floor-300);

obstacle_1_16 = platform:new(10000, floor-300);
obstacle_1_17 = platform:new(10500, floor-300);
obstacle_1_18 = platform:new(11000, floor-300);
obstacle_1_19 = obstacle_1:new(13200, floor-200);
obstacle_1_20 = platform:new(14000, floor-300);
obstacle_1_21 = platform:new(14800, floor-300);
obstacle_1_22 = platform:new(15400, floor-300);
obstacle_1_23 = platform:new(16000, floor-300);
obstacle_1_24 = platform:new(16400, floor-700);
obstacle_1_25 = platform:new(16900, floor-700);
obstacle_1_26 = platform:new(17400, floor-700);
obstacle_1_27 = platform:new(18000, floor-300);
obstacle_1_28 = platform:new(18600, floor-300);
obstacle_1_29 = platform:new(19200, floor-300);

bar_ho1_30 = bar_hor:new(20000, floor);

lifeCube1 = entity:new(1800,100)
lifeCube1:setTexture("heart.png")
lifeCube1:setVisible(true)
lifeCube1:setStatic(true)

lifeCube2 = entity:new(1700,100)
lifeCube2:setTexture("heart.png")
lifeCube2:setVisible(true)
lifeCube2:setStatic(true)

lifeCube3 = entity:new(1600,100)
lifeCube3:setTexture("heart.png")
lifeCube3:setVisible(true)
lifeCube3:setStatic(true)

coinCounter = entity:new(1920/2-240,115)
coinCounter:setTexture("coin_hud.png")
coinCounter:setTextureScale(0.8, 0.8)
coinCounter:setVisible(true)
coinCounter:setStatic(true)

coinCounterText = textObject:new(1920/2-128,100, 50, "font1.ttf")
coinCounterText:setText('0')

makeDecal(100, floor - 128, DECAL_TYPE.BUSH)
makeDecal(1300, floor - 128, DECAL_TYPE.ROCK)
makeDecal(1900, floor - 128, DECAL_TYPE.GRASS)
makeDecal(4000, floor - 128, DECAL_TYPE.GRASS)
makeDecal(8100, floor - 128, DECAL_TYPE.ROCK)
makeDecal(4700, floor - 128, DECAL_TYPE.FENCE)
makeDecal(5800, floor - 128, DECAL_TYPE.MUSHROOM_BROWN)
makeDecal(9000, floor - 128, DECAL_TYPE.MUSHROOM_RED)
makeDecal(9300, floor - 128, DECAL_TYPE.BUSH)

player1 = player:new(100, 100)

coin1 = coin:new(1400, 900)
coin2 = coin:new(1500, 900)
coin3 = coin:new(1600, 900)

coin4 = coin:new(1400, 700)
coin5 = coin:new(1500, 700)
coin6 = coin:new(1600, 700)

function resetPlayerPosition()
    controllerRumble(0.5, 1000)
    playSound("respawn.wav", 80, 0)
    player1.lives = player1.lives - 1;
    if(player1.lives == 2) then
        lifeCube3:setVisible(false)
    elseif player1.lives == 1 then
        lifeCube2:setVisible(false)
    elseif player1.lives == 0 then
        lifeCube1:setVisible(false)
    end
    player1:setXY(100,100)
end

trigger_pit_0 = trigger:new(6400, floor-10, 800, 20)
trigger_pit_0:onCollisionWith(player1, resetPlayerPosition)
trigger_pit_1 = trigger:new(10400, floor-10, 800, 20)
trigger_pit_1:onCollisionWith(player1, resetPlayerPosition)
trigger_pit_2 = trigger:new(14400, floor-10, 5600, 20)
trigger_pit_2:onCollisionWith(player1, resetPlayerPosition)
trigger_pit_3 = trigger:new(20800, floor-10, 1000, 20)
trigger_pit_3:onCollisionWith(player1, resetPlayerPosition)

spikes1 = makeSpikes(8512, floor)
spikes2 = makeSpikes(8512+128, floor)

labelTimer = 0;

--[[ label1 = label:new(270, 800, "Woa, what's going on?!", "font1.ttf", 50)
label1:hide()
label2 = label:new(300, 800, "Why am I a SQUARE????", "font1.ttf", 50)
label2:hide()
label3 = label:new(280, 800, " ", "font1.ttf", 50)
label3:hide()
label4 = label:new(350, 800, "Is this a dream?!", "font1.ttf", 50)
label4:hide()
label5 = label:new(400, 800, "I hope it is...", "font1.ttf", 50)
label5:hide()
label6 = label:new(270, 800, "wait... PLATFORMS???", "font1.ttf", 50)
label6:hide()
label7 = label:new(470, 800, "WHY???", "font1.ttf", 50)
label7:hide()
label8 = label:new(300, 800, "WHAT'S GOING ON!?", "font1.ttf", 50)
label8:hide() ]]

--[[ bouncyTriggerCounter = 0
bouncyTriggerAmount = 0
label9 = label:new(270, 800, "Hehe, it's bouncy", "font1.ttf", 50)
label9:hide()

label10 = label:new(270, 800, "I should probably stop...", "font1.ttf", 50)
label10:hide() ]]

--PAUSE MENU=========================================================================
pauseBG = entity:new(0, floor)
--pauseBG:setTexture("fortress.webp")
pauseBG:setVisible(false)
pauseBG:setStatic(true)
pauseBG:setY(1080)

--Buttons and their update functions
----@Key
all_buttons_key = "MOUSE1"

----@Audio
--all_buttons_sound = "buttonSelected.wav"

--CONTINUE BUTTON
button_continue = button:new(50, 500, "Continue", "font1.ttf", 80)
button_continue:setClickKey(all_buttons_key)
button_continue:setTexture("button01.png")
button_continue:setTextureScale(2, 2)
button_continue:setMarginX(474)
button_continue:setMarginY(92)
button_continue:hide()

function button_continue:onClick()
    unpauseGame()
end

function button_continue:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 100, 100, 255)
        self:setTexture("button01_hover.png")
    end
end

function button_continue:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
    self:setTexture("button01.png")
end

--EXIT BUTTON
button_exit = button:new(50, 600, "Exit", "font1.ttf", 80)
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
        self.text:setTextColor(255, 100, 100, 255)
    end
end

function button_exit:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end

--FUNCTIONS FOR PAUSING THE GAME
function pauseGame()
    setOverlay(" ", 1)
    --setDarkness(0)
    pausePhysics()
    setGravity(0)
    pauseBG:setVisible(true)
    button_continue:show()
    button_exit:show()
    showMouseCursor()
    pause = 1
end

function unpauseGame()
    unpausePhysics()
    setGravity(gravity)
    pauseBG:setVisible(false)
    button_continue:hide()
    button_exit:hide()
    hideMouseCursor()
    --setBackground("fortress.webp")
    pause = 0
end

--END PAUSE MENU

use("playerSwitch.lua")

function loop()
    --log(INFO, "leftTrigger: "..tostring(leftTrigger()).." rightTrigger: "..tostring(rightTrigger())..
    --" rightStickHor: "..tostring(rightStickHorizontal()).." rightStickVer: "..tostring(rightStickVertical())..
    --" leftStickHor: "..tostring(leftStickHorizontal()).." leftStickVer: "..tostring(leftStickVertical()))

    if(pause == 0) then
        if keyPressed("PAUSE") then
            pauseGame()
        end
        coin1:update()
        coin2:update()
        coin3:update()
        coin4:update()
        coin5:update()
        coin6:update()
        coinCounterText:setText(coinsCollected)

        player1:update()
        playerSwitch:update()

        obstacle_1_4:update()
        obstacle_1_5:update()
        obstacle_1_6:update()
        obstacle_1_7:update()

        obstacle_1_10:update()
        obstacle_1_11:update()
        obstacle_1_12:update()

        obstacle_1_16:update()
        obstacle_1_17:update()
        obstacle_1_18:update()

        obstacle_1_20:update()
        obstacle_1_21:update()
        obstacle_1_22:update()
        obstacle_1_23:update()
        obstacle_1_24:update()
        obstacle_1_25:update()
        obstacle_1_26:update()
        obstacle_1_27:update()
        obstacle_1_28:update()
        obstacle_1_29:update()

        trigger_pit_0:update()
        trigger_pit_1:update()
        trigger_pit_2:update()
        trigger_pit_3:update()
        spikes1:update()
        spikes2:update()

        endLevel:update()



--[[         if(labelTimer > 10 and labelTimer < 150) then
            label1:show()
            label1.text:setY(player1:getY()-150)
        elseif labelTimer > 150 and labelTimer < 300 then
            label1:hide()
            label2:show()
            label2.text:setY(player1:getY()-150)
        elseif labelTimer > 300 and labelTimer < 450 then
            label2:hide()
            label3:show()
            label3.text:setY(player1:getY()-150)
        elseif labelTimer > 450 and labelTimer < 600 then
            label3:hide()
            label4:show()
            label4.text:setY(player1:getY()-150)
        elseif labelTimer > 600 and labelTimer < 750 then
            label4:hide()
            label5:show()
            label5.text:setY(player1:getY()-150)
        elseif labelTimer > 750 and labelTimer < 850 then
            label5:hide()
            label6:show()
            label6.text:setY(player1:getY()-150)
        elseif labelTimer > 850 and labelTimer < 900 then
            label6:hide()
            label7:show()
            label7.text:setY(player1:getY()-150)
        elseif labelTimer > 900 and labelTimer < 950 then
            label7:hide()
            label8:show()
            label8.text:setY(player1:getY()-150)
        elseif labelTimer > 950 then
            label8:hide()
            player1.speed = 15
        end

        labelTimer = labelTimer + 1 ]]
        centreCamera(player1.ID)
    elseif(pause == 1) then
        -- setDarkness(0)
        button_continue:update()
        button_exit:update()
        if keyPressed("PAUSE") then
            unpauseGame()
        end
    end
end