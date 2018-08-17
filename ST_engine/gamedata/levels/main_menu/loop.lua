-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com

introFading()



if(room == "main") then
    if keyPressed("START") and darkness == 0 then
        button_newGame:onClick()
    end
    button_settings:update()
    button_exit:update()
    button_newGame:update()
elseif(room == "settings") then
    if keyPressed("BACK") then
        button_back:onClick()
    end 
    button_back:update()
    button_fullscreen:update()
    button_soundSwitch:update()
    button_switch1:update()
    button_vsync:update()
    button_switch2:update()
    button_switch3:update()
end
