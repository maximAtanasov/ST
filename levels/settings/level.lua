use("button.lua")

setLevelSize(3000, 3000)
setGravity(0)
showMouseCursor()
setBackground("textures/menu.png")

soundToggle = getVolume()
vsyncToggle = getVsync()

--Buttons and their update functions
button_back = button:new(50, 700)
button_back:setTexture("textures/buttonBack.png")
button_back:setTexW(200)
button_back:setTexH(65)

function button_back:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
        self:setTexture("textures/buttonBackSelected.png")
        if keyPressed("MOUSE1") then
            startLevel("main")
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonBack.png")
    end
end

button_soundSwitch = button:new(50, 500)
button_soundSwitch:setTexture("textures/buttonBack.png")
button_soundSwitch:setTexW(200)
button_soundSwitch:setTexH(65)

function button_soundSwitch:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
        self:setTexture("textures/buttonSoundSelected.png")
        if keyPressed("MOUSE1") then
            toggleAudio()
            playSound("sound/buttonSelected.wav")
            if soundToggle > 0 then
                soundToggle = 0
            else
                soundToggle = getVolume()
            end
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonSound.png")
    end
end

button_switch1 = button:new(300, 502)
button_switch1:setTexture("textures/buttonOn.png")

function button_switch1:update()
    if soundToggle > 0 then
        self:setTexture("textures/buttonOn.png")
    else
        self:setTexture("textures/buttonOff.png")
    end
end

button_vsync = button:new(50, 600)
button_vsync:setTexture("textures/buttonVsync.png")
button_vsync:setTexW(183)
button_vsync:setTexH(45)

function button_vsync:update()
    if mouseOver(self) then
        if self.soundPlayed == 1 then
            playSound("sound/buttonSelected.wav")
            self.soundPlayed = 0;
        end
        self:setTexture("textures/buttonVsyncSelected.png")
        if keyPressed("MOUSE1") then
            --playSound("sound/buttonSelected.wav")
            if vsyncToggle > 0 then
                vsyncToggle = 0
                vsyncOff()
            else
                vsyncToggle = 1
                vsyncOn()
            end
        end
    else
        self.soundPlayed = 1
        self:setTexture("textures/buttonVsync.png")
    end
end

button_switch2 = button:new(300, 602)
button_switch2:setTexture("textures/buttonOn.png")

function button_switch2:update()
    if vsyncToggle > 0 then
        self:setTexture("textures/buttonOn.png")
    else
        self:setTexture("textures/buttonOff.png")
    end
end

