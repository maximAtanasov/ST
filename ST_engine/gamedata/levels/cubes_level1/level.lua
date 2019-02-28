-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

use("player.lua")
use("obstacle_1.lua")
use("platform.lua")
use("bar_hor.lua")
use("trigger.lua")

pause = 0

stopAllSounds()
unpausePhysics()
enableLighting(false)

setLevelSize(150000, 3000)
floor = 1081
gravity = 18

setGravity(gravity)

setLevelFloor(floor)
setDarkness(0)
hideMouseCursor()

setBackground("bg1.png")
--setOverlay("rain.webp", 13);
--playMusic("Shades.ogg", 100, -1)
--playSound("rain.wav", 50, -1)
--Create and initialize entities

bar_ho1_1 = bar_hor:new(0, floor);
bar_ho1_2 = bar_hor:new(800, floor);
bar_ho1_3 = bar_hor:new(1600, floor);
bar_ho1_4 = bar_hor:new(2400, floor);
bar_ho1_5 = bar_hor:new(3200, floor);
bar_ho1_6 = bar_hor:new(4000, floor);
bar_ho1_7 = bar_hor:new(4800, floor);
bar_ho1_8 = bar_hor:new(5600, floor);
--bar_ho1_9 = bar_hor:new(6400, floor);

bar_ho1_10 = bar_hor:new(7200, floor);
bar_ho1_11 = bar_hor:new(8000, floor);
bar_ho1_12 = bar_hor:new(8800, floor);
bar_ho1_13 = bar_hor:new(9600, floor);
--bar_ho1_14 = bar_hor:new(10400, floor);
bar_ho1_15 = bar_hor:new(11200, floor);
bar_ho1_16 = bar_hor:new(12000, floor);
bar_ho1_17 = bar_hor:new(12800, floor);
bar_ho1_18 = bar_hor:new(13600, floor);

obstacle_1_1 = obstacle_1:new(500, floor-200);
obstacle_1_2 = obstacle_1:new(600, floor-400);
obstacle_1_2:setTexture("obstacle_2.png")
obstacle_1_3 = obstacle_1:new(800, floor-600);

player1 = player:new(100, 100);

obstacle_1_4 = platform:new(2000, floor-400);
obstacle_1_4.height = floor - 400

obstacle_1_5 = platform:new(3000, floor-150);
obstacle_1_5.height = floor - 150


obstacle_1_6 = platform:new(3500, floor-360);
obstacle_1_6.height = floor-360

obstacle_1_7 = obstacle_1:new(4000, floor-200);
obstacle_1_8 = obstacle_1:new(5280, floor-200);
--obstacle_1_9 = obstacle_1:new(5500, floor-200);

obstacle_1_10 = platform:new(6100, floor-300);
obstacle_1_10.height = floor-300

obstacle_1_10_dialogTrigger = trigger:new(6130, floor-260);
obstacle_1_10_dialogTrigger:setCollision(0,0, 360, 60)
obstacle_1_10_dialogTrigger:setTexW(400)
obstacle_1_10_dialogTrigger:setTexH(60)

obstacle_1_11 = platform:new(6700, floor-300);
obstacle_1_11.height = floor - 300

obstacle_1_12 = platform:new(7250, floor-300);
obstacle_1_12.height = floor - 300

obstacle_1_13 = obstacle_1:new(8000, floor-200);
obstacle_1_14 = obstacle_1:new(8800, floor-200);
--obstacle_1_15 = obstacle_1:new(9500, floor-200);

trigger_pit_0 = trigger:new(6400, floor-10)
trigger_pit_0:setCollision(0,0, 800, 20);

obstacle_1_16 = platform:new(10000, floor-300);
obstacle_1_16.height = floor -300

obstacle_1_17 = platform:new(10500, floor-300);
obstacle_1_17.height = floor - 300

obstacle_1_18 = platform:new(11000, floor-300);
obstacle_1_18.height = floor - 300

trigger_pit_1 = trigger:new(10400, floor-10)
trigger_pit_1:setCollision(0,0, 800, 20);

labelTimer = 0;

label1 = label:new(270, 800, "Woa, what's going on?!", "font1.ttf", 50)
label1:hide()
label2 = label:new(270, 800, "Why am I a CUBE????", "font1.ttf", 50)
label2:hide()
label3 = label:new(270, 800, "A square actually...", "font1.ttf", 50)
label3:hide()
label4 = label:new(270, 800, "Is this a dream?!", "font1.ttf", 50)
label4:hide()
label4 = label:new(270, 800, "Is this a dream?!", "font1.ttf", 50)
label4:hide()
label5 = label:new(270, 800, "I hope it is...", "font1.ttf", 50)
label5:hide()
label6 = label:new(270, 800, "wait... PLATFORMS???", "font1.ttf", 50)
label6:hide()
label7 = label:new(270, 800, "WHY???", "font1.ttf", 50)
label7:hide()
label8 = label:new(270, 800, "WHAT'S GOING ON!?", "font1.ttf", 50)
label8:hide()

bouncyTriggerCounter = 0
bouncyTriggerAmount = 0
label9 = label:new(270, 800, "Hehe, it's bouncy", "font1.ttf", 50)
label9:hide()

label10 = label:new(270, 800, "I should probably stop...", "font1.ttf", 50)
label10:hide()

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
    end
end

function button_continue:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
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


function loop()
    --log(INFO, "leftTrigger: "..tostring(leftTrigger()).." rightTrigger: "..tostring(rightTrigger())..
    --" rightStickHor: "..tostring(rightStickHorizontal()).." rightStickVer: "..tostring(rightStickVertical())..
    --" leftStickHor: "..tostring(leftStickHorizontal()).." leftStickVer: "..tostring(leftStickVertical()))

    if(pause == 0) then
        if keyPressed("PAUSE") then
            pauseGame()
        end
        obstacle_1_4:update()
        obstacle_1_5:update()
        obstacle_1_6:update()

        obstacle_1_10:update()
        obstacle_1_11:update()
        obstacle_1_12:update()

        obstacle_1_16:update()
        obstacle_1_17:update()
        obstacle_1_18:update()

        player1:update()

        if(labelTimer > 10 and labelTimer < 150) then
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
        labelTimer = labelTimer + 1
        if bouncyTriggerAmount == 0 then
            if player1:overObject(obstacle_1_10_dialogTrigger) then
                label9:show()
                bouncyTriggerAmount = 1
            end
        elseif bouncyTriggerAmount == 1 then
            label9.text:setY(player1:getY()-150)
            bouncyTriggerCounter = bouncyTriggerCounter + 1
        elseif bouncyTriggerAmount == 2 then
            if player1:overObject(obstacle_1_10_dialogTrigger) then
                label9:hide()
                label10:show()
            end
            label10.text:setY(player1:getY()-150)
            bouncyTriggerCounter = bouncyTriggerCounter + 1
        end
        if bouncyTriggerCounter > 100 then
            bouncyTriggerAmount = 2
        end
        if bouncyTriggerCounter > 400 then
            bouncyTriggerAmount = 3
            label10:hide()
            label9:hide()
        end
        if player1:overObject(trigger_pit_1) or player1:overObject(trigger_pit_0) then
            controllerRumble(1, 1)
            player1:setXY(100, 100)
        end
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