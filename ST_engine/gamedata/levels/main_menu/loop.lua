--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

if darkness > 0 then
    darkness = darkness - 1
    setDarkness(darkness)
end

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
