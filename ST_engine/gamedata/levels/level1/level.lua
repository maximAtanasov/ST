-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

use("player.lua")
use("trigger.lua")
use("platform.lua")
use("block.lua")
use("infoBlock.lua")
use("floatBlock.lua")
use("crate.lua")
use("coin.lua")
use("water.lua")
use("decal.lua")
use("spikes.lua")
use("pauseMenu.lua")
pause = 0

--TODO: Experiment with different assets sizes on different resolutions
screenWidth, screenHeight = getWindowResolution()
setInternalResolution(screenWidth, screenHeight)

floor = screenHeight

gravity = 18
coinsCollected = 0;

--level init
setLevelFloor(floor)
setDarkness(0)
hideMouseCursor()
stopAllSounds()
unpausePhysics()
enableLighting(false)
setGravity(gravity)
setLevelSize(150000, 3000)

setBackground("bg2_1.png", 0, 0)
setBackground("bg2_2.png", 1, 3)
setBackground("bg2_3.png", 2, 4)
setBackground("bg2_4.png", 3, 3)


playMusic("song1.ogg", 50, -1)
--playSound("rain.wav", 50, -1)
--Create and initialize entities

levelBeginBlock = entity:new(-10, floor)
levelBeginBlock:setAffectedByPhysics(true)
levelBeginBlock:setCollision(0,0,10, screenHeight)

crate:new(500, floor-200);
crate:new(540, floor-400);
crate:new(800, floor-600);

local platform_4 = platform:new(2000, floor-400);
local platform_5 = platform:new(2700, floor-200);
local platform_6 = platform:new(3500, floor-400);
local platform_7 = platform:new(4050, floor-290);

crate:new(5650, floor-300);

local platform_10 = platform:new(6100, floor-300);

platform_10_dialogTrigger = trigger:new(6130, floor-260, 360, 60);

local platform_11 = platform:new(6700, floor-300);
local platform_12 = platform:new(7250, floor-300);

local platform_16 = platform:new(9500, floor-300);
local platform_17 = platform:new(10100, floor-300);
local platform_18 = platform:new(10700, floor-300);
local platform_19 = platform:new(13200, floor-200);
local platform_20 = platform:new(14000, floor-300);
local platform_21 = platform:new(14800, floor-300);
local platform_22 = platform:new(15400, floor-300);
local platform_23 = platform:new(16000, floor-300);
local platform_24 = platform:new(16400, floor-700);
local platform_25 = platform:new(16900, floor-700);
local platform_26 = platform:new(17400, floor-700);
local platform_27 = platform:new(18000, floor-300);
local platform_28 = platform:new(18600, floor-300);
local platform_29 = platform:new(19200, floor-300);

makeBlock(20000, floor, BLOCK_TYPE.GRASS)

lifeCube1 = entity:new(screenWidth - 200,100)
lifeCube1:setTexture("heart.png")
lifeCube1:setTextureScale(0.8, 0.8)
lifeCube1:setVisible(true)
lifeCube1:setStatic(true)

lifeCube2 = entity:new(screenWidth - 300,100)
lifeCube2:setTexture("heart.png")
lifeCube2:setTextureScale(0.8, 0.8)
lifeCube2:setVisible(true)
lifeCube2:setStatic(true)

lifeCube3 = entity:new(screenWidth - 400,100)
lifeCube3:setTexture("heart.png")
lifeCube3:setTextureScale(0.8, 0.8)
lifeCube3:setVisible(true)
lifeCube3:setStatic(true)

coinCounter = entity:new(screenWidth/2-240,115)
coinCounter:setTexture("coin_hud.png")
coinCounter:setTextureScale(0.8, 0.8)
coinCounter:setVisible(true)
coinCounter:setStatic(true)

local coinCounterText = textObject:new(screenWidth/2-128,100, 50, "font1.ttf")
coinCounterText:setText('0')

makeDecal(100, floor - 128, DECAL_TYPE.BUSH)
makeDecal(1300, floor - 128, DECAL_TYPE.ROCK)
makeDecal(1900, floor - 128, DECAL_TYPE.GRASS)
makeDecal(4000, floor - 128, DECAL_TYPE.GRASS)
makeDecal(8100, floor - 128, DECAL_TYPE.ROCK)
makeDecal(8250, floor - 128, DECAL_TYPE.GRASS)
makeDecal(5800, floor - 128, DECAL_TYPE.MUSHROOM_BROWN)
makeDecal(9000, floor - 128, DECAL_TYPE.MUSHROOM_RED)
makeDecal(9300, floor - 128, DECAL_TYPE.BUSH)

player1 = player:new(100, 100)

makeDecal(4700, floor - 128, DECAL_TYPE.FENCE)
makeDecal(4830, floor - 128, DECAL_TYPE.FENCE)
makeDecal(4960, floor - 128, DECAL_TYPE.FENCE_BROKEN)

local infoBlock1 = infoBlock:new(250, screenHeight - 450)

local block1 = floatBlock:new(1300, screenHeight - 400)
local block2 = floatBlock:new(1429, screenHeight - 400)
local block3 = floatBlock:new(1558, screenHeight - 400)
local block4 = floatBlock:new(1687, screenHeight - 400)
local block5 = floatBlock:new(9200, screenHeight - 200)

local coin1 = coin:new(1400, screenHeight - 800)
local coin2 = coin:new(1500, screenHeight - 800)
local coin3 = coin:new(1600, screenHeight - 800)

local coin4 = coin:new(1400, screenHeight - 600)
local coin5 = coin:new(1500, screenHeight - 600)
local coin6 = coin:new(1600, screenHeight - 600)

local coin7 = coin:new(9800, screenHeight - 600)
local coin8 = coin:new(9900, screenHeight - 600)
local coin9 = coin:new(10000, screenHeight - 600)

local coin10 = coin:new(10100, screenHeight - 600)
local coin11 = coin:new(10200, screenHeight - 600)
local coin12 = coin:new(10300, screenHeight - 600)

local coin13 = coin:new(10000, screenHeight - 200)
local coin14 = coin:new(10100, screenHeight - 200)
local coin15 = coin:new(10200, screenHeight - 200)

local coin16 = coin:new(10300, screenHeight - 200)
local coin17 = coin:new(10400, screenHeight - 200)
local coin18 = coin:new(10500, screenHeight - 200)

local water_1 = water:new(6272, floor);
local water_2 = water:new(6784, floor);
local water_3 = water:new(7296, floor);
local water_4 = water:new(7808, floor);

makeBlock(0, floor, BLOCK_TYPE.GRASS)
makeBlock(512, floor, BLOCK_TYPE.GRASS)
makeBlock(1024, floor, BLOCK_TYPE.GRASS)
makeBlock(1536, floor, BLOCK_TYPE.GRASS)
makeBlock(2048, floor, BLOCK_TYPE.GRASS)
makeBlock(2560, floor, BLOCK_TYPE.GRASS)
makeBlock(3072, floor, BLOCK_TYPE.GRASS)
makeBlock(3584, floor, BLOCK_TYPE.GRASS)
makeBlock(4096, floor, BLOCK_TYPE.GRASS)
makeBlock(4608, floor, BLOCK_TYPE.GRASS)
makeBlock(5120, floor, BLOCK_TYPE.GRASS)
makeBlock(5632, floor, BLOCK_TYPE.GRASS)
makeBlock(6144, floor, BLOCK_TYPE.GRASS_EDGE_WATER)
makeBlock(8000, floor, BLOCK_TYPE.GRASS_ROUNDED)
makeBlock(8768, floor, BLOCK_TYPE.GRASS_ROUNDED_LEFT)
makeBlock(9280, floor, BLOCK_TYPE.GRASS)
makeBlock(9792, floor, BLOCK_TYPE.GRASS)
makeBlock(10304, floor, BLOCK_TYPE.GRASS)
makeBlock(10816, floor, BLOCK_TYPE.GRASS)
makeBlock(11328, floor, BLOCK_TYPE.GRASS)

function resetPlayerPosition()
    controllerRumble(0.5, 1000)
    playSound("death.wav", 100, 0)
    player1.lives = player1.lives - 1;
    if(player1.lives == 2) then
        lifeCube3:setVisible(false)
    elseif player1.lives == 1 then
        lifeCube2:setVisible(false)
    elseif player1.lives == 0 then
        lifeCube1:setVisible(false)
    end
    player1:setXY(100,100)
    player1:moveRight(5)
end

local trigger_pit_1 = trigger:new(10400, floor-10, 800, 20)
trigger_pit_1:onCollisionWith(player1, resetPlayerPosition)
local trigger_pit_2 = trigger:new(14400, floor-10, 5600, 20)
trigger_pit_2:onCollisionWith(player1, resetPlayerPosition)
local trigger_pit_3 = trigger:new(20800, floor-10, 1000, 20)
trigger_pit_3:onCollisionWith(player1, resetPlayerPosition)

local spikes1 = makeSpikes(8512, floor)
local spikes2 = makeSpikes(8512+128, floor)

--PAUSE MENU=========================================================================
pauseBG = entity:new(0, floor)
--pauseBG:setTexture("fortress.webp")
pauseBG:setVisible(false)
pauseBG:setStatic(true)
pauseBG:setY(screenHeight)

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
        --playSound(all_buttons_sound, 100, 0)
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
        --playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 100, 100, 255)
    end
end

function button_exit:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end

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
        coin7:update()
        coin8:update()
        coin9:update()
        coin10:update()
        coin11:update()
        coin12:update()
        coin13:update()
        coin14:update()
        coin15:update()
        coin16:update()
        coin17:update()
        coin18:update()

        coinCounterText:setText(coinsCollected)

        infoBlock1:update()
        block1:update()
        block2:update()
        block3:update()
        block4:update()
        block5:update()

        player1:update()
        playerSwitch:update(player1)

        platform_4:update()
        platform_5:update()
        platform_6:update()
        platform_7:update()
        platform_10:update()
        platform_11:update()
        platform_12:update()
        platform_16:update()
        platform_17:update()
        platform_18:update()
        platform_19:update()
        platform_20:update()
        platform_21:update()
        platform_22:update()
        platform_23:update()
        platform_24:update()
        platform_25:update()
        platform_26:update()
        platform_27:update()
        platform_28:update()
        platform_29:update()

        trigger_pit_1:update()
        trigger_pit_2:update()
        trigger_pit_3:update()

        spikes1:update()
        spikes2:update()

        water_1:update()
        water_2:update()
        water_3:update()
        water_4:update()

        centreCamera(player1.ID)
    elseif(pause == 1) then
        button_continue:update()
        button_exit:update()
        if keyPressed("PAUSE") then
            unpauseGame()
        end
    end
end