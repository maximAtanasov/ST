-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com

if startCounter < 100 then
    startCounter = startCounter + 1
    if keyPressed("START") or keyPressed("ESCAPE") or keyPressed("START_C") then
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

if keyPressed("START") or keyPressed("ESCAPE") or keyPressed("START_C") then
    unloadLevel("author_splash")
    startLevel("main_menu")
end