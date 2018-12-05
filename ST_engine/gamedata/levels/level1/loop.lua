-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com


--code is run in a loop inside the main executable

log(INFO, "leftTrigger: "..tostring(leftTrigger()).." rightTrigger: "..tostring(rightTrigger()))

if(pause == 0) then
    if keyPressed("PAUSE") then
        pauseGame()
    end

    player1:update()
    coin1:update()
    coin2:update()
    coin3:update()
    coin4:update()
    coin5:update()
    coin6:update()
    crystal_green1:update()

    coin7:update()
    coin8:update()
    coin9:update()
    coin10:update()
    coin11:update()
    coin12:update()
    wolf1:update()

    centreCamera(player1.ID)
    --thunderclap
    local random = math.random
    if random(1, 800) == 50 then
        if random(1, 4) ~= 3 then
            playSound("thunderclap.wav", 100, 0)
        else
            playSound("thunderclap2.wav", 75, 0)
        end
        setDarkness(0)
    else
        setDarkness(darkness)
    end

elseif(pause == 1) then
    setDarkness(0)
    button_continue:update()
    button_exit:update()
    if keyPressed("PAUSE") then
        unpauseGame()
    end
end
