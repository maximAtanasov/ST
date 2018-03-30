
--Load the appropriate language configuration
if language == "russian" then
    use("language_russian.lua")
elseif language == "english" then
    use("language_english.lua")
end

label = createButton(960-LABEL_OFFSET, 870, LABEL_TEXT, FONT, FONT_SIZE)

setGravity(0)
setBackground("splash.png")
setDarkness(254)
loadLevel("main_menu")
hideMouseCursor()
splash = 255
splashUp = 0