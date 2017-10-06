use("button.lua")

setLevelSize(3000, 3000)
setGravity(0)
showMouseCursor()
setBackground("textures/menu.png")
loadLevel("level1") --preload level1
loadLevel("settings") --preload settings menu

--Buttons and their update functions

--New Game button
button_newGame = button:new(50, 500)
button_newGame:setTexture("textures/buttonNewGame.png")
button_newGame:setTexW(474)
button_newGame:setTexH(92)

function button_newGame:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
        self:setTexture("textures/buttonNewGameSelected.png")
        if keyPressed("MOUSE1") then
            startLevel("level1")
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonNewGame.png")
    end
end


--Settings button
button_settings = button:new(50, 600)
button_settings:setTexture("textures/buttonSettings.png")
button_settings:setTexW(479)
button_settings:setTexH(98)

function button_settings:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
        self:setTexture("textures/buttonSettingsSelected.png")
        if keyPressed("MOUSE1") then
            startLevel("settings")
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonSettings.png")
    end
end


--Exit button
button_exit = button:new(50, 700)
button_exit :setTexture("textures/buttonExit.png")
button_exit:setTexW(224)
button_exit:setTexH(85)

function button_exit:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
    self:setTexture("textures/buttonExitSelected.png")
        if keyPressed("MOUSE1") then
            endGame()
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonExit.png")
    end
end
