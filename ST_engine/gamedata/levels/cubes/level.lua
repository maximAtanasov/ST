-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

enableLighting(true)
stopAllSounds()
stopMusic()

label = label:new(600, 500, "So, a game about a cube?", "font1.ttf", 50)
label1 = label:new(400, 600, "Actually it’s a game about a bunch of cubes", "font1.ttf", 50)

setGravity(0)
setDarkness(254)
hideMouseCursor()
loadLevel("cubes_level1")
splash = 255
splashUp = 0
dontSkip = false

function loop()
    if splash > 0 then
        splash = splash - 1
        setDarkness(splash)
    elseif splashUp < 255 then
        splashUp = splashUp + 1
        setDarkness(splashUp)
    else
        startLevel("cubes_level1")
    end

    if keyPressed("START") then
        if dontSkip == false then
            label:new(600, 1000, "C'mon, don't skip my intro...", "font1.ttf", 50)
            dontSkip = true
         end
    end

    if(rightTrigger() > 20000) then
        startLevel("cubes_level1")
    end
end