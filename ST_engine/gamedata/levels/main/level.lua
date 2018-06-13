--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com


--Load the appropriate language configuration
if language == "russian" then
    use("language_russian.lua")
elseif language == "english" then
    use("language_english.lua")
end

label = label:new(960-LABEL_OFFSET, 870, LABEL_TEXT, FONT, FONT_SIZE)

setGravity(0)
setBackground("splash.png")
setDarkness(254)
loadLevel("main_menu")
hideMouseCursor()
splash = 255
splashUp = 0
