if splash > 0 then
    splash = splash - 1
    setDarkness(splash)
elseif splashUp < 255 then
    splashUp = splashUp + 1
    setDarkness(splashUp)
else
    startLevel("main_menu")
end
