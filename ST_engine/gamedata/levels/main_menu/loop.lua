
if darkness > 0 then
    darkness = darkness - 1
    setDarkness(darkness)
end

if(room == "main") then
    button_settings:update()
    button_exit:update()
    button_newGame:update()
elseif(room == "settings") then
    button_back:update()
    button_fullscreen:update()
    button_soundSwitch:update()
    button_switch1:update()
    button_vsync:update()
    button_switch2:update()
    button_switch3:update()
end
