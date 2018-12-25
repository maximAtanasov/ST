-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com


loadLevel("level1")

enableLighting(true)
setLevelSize(3000, 3000)
setGravity(0)
pausePhysics()
showMouseCursor()
setBackground("menu.webp")
setDarkness(255)

darkness = 255
initialVolume = getMusicVolume()
function introFading()
    if darkness > 0 then
        darkness = darkness - 1
        setDarkness(darkness)
        if(darkness % 4 == 0) then
            if initialVolume > 36 then
                initialVolume = initialVolume - 1
                setMusicVolume(initialVolume)
            end
        end
    elseif darkness == 0 then
        enableLighting(false)
        introFading = function () end
    end
end


--Load the appropriate language configuration
if language == "russian" then
    use("language_russian.lua")
elseif language == "english" then
    use("language_english.lua")
end


room = "main"
--Buttons and their update functions

--The click key used for all buttons - the following annotion tells our interpreter that
--the string we declare is going to be used as a key

----@Key
all_buttons_key = "MOUSE1"

----@Audio
all_buttons_sound = "buttonSelected.wav"

--New Game button
button_newGame = button:new(50, 500, NEW_GAME_TEXT, FONT, 80)
button_newGame:setMarginX(474)
button_newGame:setMarginOffsetY(VERTICAL_OFFSET)
button_newGame:setMarginY(92)
button_newGame:setClickKey(all_buttons_key)


function button_newGame:onClick()
    startLevel("level1")
end

function button_newGame:onHover()
    if(self.soundPlayed == 1) then
        playSound("buttonSelected.wav", 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_newGame:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end


--Settings button
button_settings = button:new(50, 600, SETTINGS_TEXT, FONT, 80)
button_settings:setMarginX(479)
button_settings:setMarginOffsetY(VERTICAL_OFFSET)
button_settings:setMarginY(98)
button_settings:setClickKey(all_buttons_key)

function button_settings:onClick()
    room = "settings"
    button_newGame:hide()
    button_settings:hide()
    button_exit:hide()
    button_back:show()
    button_soundSwitch:show()
    button_fullscreen:show()
    button_switch1:show()
    button_vsync:show()
    button_switch2:show()
    button_switch3:show()
    button_languageSelection:show()
    button_languageSelectionLabel:show()
end

function button_settings:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_settings:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end


--Exit button
button_exit = button:new(50, 700, EXIT_TEXT, FONT, 80)
button_exit:setMarginX(224)
button_exit:setMarginOffsetY(VERTICAL_OFFSET)
button_exit:setMarginY(85)
button_exit:setClickKey(all_buttons_key)

function button_exit:onClick()
    endGame()
end

function button_exit:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_exit:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end


--===================================================================
--SETTINGS

--Buttons and their update functions

--SOUND BUTTON
button_soundSwitch = button:new(50, 500, SOUND_TEXT, FONT, 70)
button_soundSwitch:setMarginX(240)
button_soundSwitch:setMarginOffsetY(VERTICAL_OFFSET)
button_soundSwitch:setMarginY(65)
button_soundSwitch:hide()
button_soundSwitch:setClickKey(all_buttons_key)

function button_soundSwitch:onClick()
    if isAudioEnabled() then
        setAudioEnabled(false)
    else
        setAudioEnabled(true)
    end
    playSound(all_buttons_sound, 100, 0)
end

function button_soundSwitch:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_soundSwitch:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end

--ON LABEL FOR SOUND
button_switch1 = button:new(ON1_OFFSET, 500, ON_TEXT, FONT, 50)
button_switch1:hide()

function button_switch1:update()
    if isAudioEnabled() then
        button_switch1:setText(ON_TEXT)
    else
        button_switch1:setText(OFF_TEXT)
    end
end

--LANGUAGE SELECTION
button_languageSelection = button:new(50, 600, LANGUAGE_TEXT, FONT, 70)
button_languageSelection:setMarginX(LANGUAGE_MARGIN)
button_languageSelection:setMarginY(65)
button_languageSelection:setMarginOffsetY(VERTICAL_OFFSET)
button_languageSelection:hide()
button_languageSelection:setClickKey(all_buttons_key)

function button_languageSelection:onClick()
    if(language == "russian") then
        setLanguage("english")
    else
        setLanguage("russian")
    end
    playSound(all_buttons_sound, 100, 0)
    startLevel("main_menu")
end

function button_languageSelection:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_languageSelection:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end

--LANGUAGE NAME LABEL
button_languageSelectionLabel = label:new(LANUAGE_OFFSET, 600, language, FONT, 50)
button_languageSelectionLabel:hide()
if language == "english" then
    button_languageSelectionLabel:setText("English")
else
    button_languageSelectionLabel:setText("Русский")
end

--VSYNC button
button_vsync = button:new(50, 700, VSYNC_TEXT, FONT, 70)
button_vsync:setMarginX(VSYNC_MARGINS)
button_vsync:setMarginOffsetY(VERTICAL_OFFSET)
button_vsync:setMarginY(60)
button_vsync:hide()
button_vsync:setClickKey(all_buttons_key)

function button_vsync:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_vsync:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end

function button_vsync:onClick()
    if getVsyncState() then
        setVsync(false)
        button_switch2:uncheck()
    else
        setVsync(true)
        button_switch2:check()
    end
end

--ON SWITCH FOR VSYNC
 
button_switch2 = checkbox:new(VSYNC_CHECKBOX, 720 - VSYNC_CHECKBOX_Y_OFFSET)
button_switch2:hide()
button_switch2:setClickKey(all_buttons_key)
if getVsyncState() then
    button_switch2:check()
else
    button_switch2:uncheck()    
end

function button_switch2:onClick()
    if getVsyncState() then
        setVsync(false)
        self:uncheck()
    else
        setVsync(true)
        self:check()
    end
end

--Fullscreen button

button_fullscreen = button:new(50, 800, FULLSCREEN_TEXT, FONT, 70)
button_fullscreen:setMarginX(FULLSCREEN_MARGINS)
button_fullscreen:setMarginOffsetY(VERTICAL_OFFSET)
button_fullscreen:setMarginY(60)
button_fullscreen:hide()
button_fullscreen:setText(FULLSCREEN_TEXT)
button_fullscreen:setClickKey(all_buttons_key)

function button_fullscreen:onClick()
    if getFullscreenStatus() == true then
        setFullscreen(false)
    else
        setFullscreen(true)
    end
end

function button_fullscreen:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_fullscreen:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end


--ON SWITCH FOR FULLSCREEN
button_switch3 = button:new(ON3_OFFSET, 800, ON_TEXT, FONT, 50)
button_switch3:hide()

function button_switch3:update()
    if getFullscreenStatus() == true then
        button_switch3:setText(ON_TEXT)
    else
        button_switch3:setText(OFF_TEXT)
    end
end

--BACK BUTTON
button_back = button:new(50, 900, BACK_TEXT, FONT, 70)
button_back:setMarginX(200)
button_back:setMarginOffsetY(VERTICAL_OFFSET)
button_back:setMarginY(65)
button_back:hide()
button_back:setClickKey(all_buttons_key)

function button_back:onClick()
    room = "main"
    button_newGame:show()
    button_settings:show()
    button_exit:show()
    button_back:hide()
    button_fullscreen:hide()
    button_soundSwitch:hide()
    button_switch1:hide()
    button_vsync:hide()
    button_switch2:hide()
    button_switch3:hide()
    button_languageSelection:hide()
    button_languageSelectionLabel:hide()
end

function button_back:onHover()
    if(self.soundPlayed == 1) then
        playSound(all_buttons_sound, 100, 0)
        self.soundPlayed = 0
        self.text:setTextColor(255, 50, 50, 255)
    end
end

function button_back:onNothing()
    self.soundPlayed = 1
    self.text:setTextColor(255, 255, 255, 255)
end

function loop()
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
        button_languageSelection:update()
        button_languageSelectionLabel:update()
    end
end