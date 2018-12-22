-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com


enableLighting(true)

--Load the appropriate language configuration
if language == "russian" then
    use("language_russian.lua")
elseif language == "english" then
    use("language_english.lua")
end

setBackground(" ")
setGravity(0)
setDarkness(254)
hideMouseCursor()
splash = 255
splashUp = 0

label:new(960-LABEL_OFFSET, 565, LABEL_TEXT, FONT, FONT_SIZE)

startCounter = 0
loadLevel("main_menu")

function loop()
    if startCounter < 100 then
        startCounter = startCounter + 1
        if keyPressed("START") then
            startCounter = 100
        end
        return
    end

    if splash > 0 then
        splash = splash - 1
        setDarkness(splash)
    elseif splashUp < 255 then
        splashUp = splashUp + 1
        setDarkness(splashUp)
    else
        unloadLevel("author_splash")
        startLevel("main_menu")
    end

    if keyPressed("START") then
        unloadLevel("author_splash")
        startLevel("main_menu")
    end
end