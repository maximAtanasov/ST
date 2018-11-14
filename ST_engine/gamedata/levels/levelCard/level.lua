--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

setWindowResolution(432, 768)
setFullscreen(false)
setFullscreen(false)
setInternalResolution(1080, 1920)
showMouseCursor()
stopMusic();
stopAllSounds();

use("card.lua")

pause = 0

baseCardHeight = 2200

enableLighting(false)
stopAllSounds()
setLevelSize(3000, 3000)
setBackground("table.png")

card1 = card:new(0, baseCardHeight)
card2 = card:new(120, baseCardHeight)
card2:setTexture("card2.png")
card3 = card:new(240, baseCardHeight)
card3:setTexture("card3.png")
card4 = card:new(360, baseCardHeight)
card4:setTexture("card4.png")
card5 = card:new(480, baseCardHeight)
card5:setTexture("card5.png")
card6 = card:new(600, baseCardHeight)
card6:setTexture("card6.png")
card6:setTexW(500)

cardsInHand = {}

table.insert(cardsInHand, card1)
table.insert(cardsInHand, card2)
table.insert(cardsInHand, card3)
table.insert(cardsInHand, card4)
table.insert(cardsInHand, card5)
table.insert(cardsInHand, card6)

function cardsInHand:update()
    local i = 1
    while i <= 6 do
        self[i]:update()
        if self[i].goingUp == true then
            self[i]:setTexW(500)
            self[i].goingRight = false
            local j = i+1
            while j <= 6 do
                self[j].goingRight = true
                self[j]:setTexW(120)
                j = j+1
            end
            local k = i - 1
            while k >= 1 do
                self[k].goingRight = false
                self[k]:setTexW(120)
                k = k-1
            end
            card6:setTexW(500)
            if keyPressed("SPACE") then
                self[i].play = true
            end
        end
        i = i + 1
    end
end

----@Key
all_buttons_key = "MOUSE1"

----@Audio
all_buttons_sound = "buttonSelected.wav"
