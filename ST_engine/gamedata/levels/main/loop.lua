--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

if splash > 0 then
    splash = splash - 1
    setDarkness(splash)
elseif splashUp < 255 then
    splashUp = splashUp + 1
    setDarkness(splashUp)
else
    startLevel("main_menu")
end
