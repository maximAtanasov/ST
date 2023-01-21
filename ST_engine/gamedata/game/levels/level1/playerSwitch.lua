playerSwitch = entity:new(screenWidth/2-580, screenHeight/2-64)
playerSwitch:setTexture("player_switch.png")
playerSwitch:setVisible(false)
playerSwitch:setStatic(true)

playerSwitch.activePlayer = PLAYER_TYPE.BLUE
playerSwitch.activePlayerBlue = entity:new(screenWidth/2-256, screenHeight/2-64)
playerSwitch.activePlayerBlue:setTexture("hudPlayer_blue.png")
playerSwitch.activePlayerBlue:setStatic(true)
playerSwitch.activePlayerBlue:setVisible(false)

playerSwitch.activePlayerBeige = entity:new(screenWidth/2-384, screenHeight/2-64)
playerSwitch.activePlayerBeige:setTexture("hudPlayer_beige.png")
playerSwitch.activePlayerBeige:setStatic(true)
playerSwitch.activePlayerBeige:setVisible(false)

playerSwitch.activePlayerGreen = entity:new(screenWidth/2-128, screenHeight/2-64)
playerSwitch.activePlayerGreen:setTexture("hudPlayer_green.png")
playerSwitch.activePlayerGreen:setStatic(true)
playerSwitch.activePlayerGreen:setVisible(false)

playerSwitch.activePlayerPink = entity:new(screenWidth/2 - 7, screenHeight/2-64)
playerSwitch.activePlayerPink:setTexture("hudPlayer_pink.png")
playerSwitch.activePlayerPink:setStatic(true)
playerSwitch.activePlayerPink:setVisible(false)

playerSwitch.activePlayerYellow = entity:new(screenWidth/2 + 121, screenHeight/2-64)
playerSwitch.activePlayerYellow:setTexture("hudPlayer_yellow.png")
playerSwitch.activePlayerYellow:setStatic(true)
playerSwitch.activePlayerYellow:setVisible(false)

playerSwitch.counter = 0
playerSwitch.playerIndex = 2;
--playerSwitch = {playerSwitch.activePlayerBeige, playerSwitch.activePlayerBlue, playerSwitch.activePlayerGreen,
--playerSwitch.activePlayerPink, playerSwitch.activePlayerYellow }


function playerSwitch:update(player)
    leftSwitch = keyPressed("PLAYER_SWITCH_LEFT")
    rightSwitch = keyPressed("PLAYER_SWITCH_RIGHT")
    if leftSwitch or rightSwitch then
        playSound("switch.wav", 100, 0)
        if(leftSwitch) then
            if(self.playerIndex > 1) then
                self.playerIndex = self.playerIndex - 1
            else
                self.playerIndex = 5
            end
        elseif rightSwitch then
            if(self.playerIndex < 5) then
                self.playerIndex = self.playerIndex + 1
            else
                self.playerIndex = 1
            end
        end
        self:setVisible(true)
        if(self.playerIndex == 1) then
            player:setPlayerType(PLAYER_TYPE.BEIGE)
            self.activePlayerBeige:setVisible(true)
            self.activePlayerBlue:setVisible(false)
            self.activePlayerGreen:setVisible(false)
            self.activePlayerPink:setVisible(false)
            self.activePlayerYellow:setVisible(false)
        elseif(self.playerIndex == 2) then
            player:setPlayerType(PLAYER_TYPE.BLUE)
            self.activePlayerBeige:setVisible(false)
            self.activePlayerBlue:setVisible(true)
            self.activePlayerGreen:setVisible(false)
            self.activePlayerPink:setVisible(false)
            self.activePlayerYellow:setVisible(false)
        elseif(self.playerIndex == 3) then
            player:setPlayerType(PLAYER_TYPE.GREEN)
            self.activePlayerBeige:setVisible(false)
            self.activePlayerBlue:setVisible(false)
            self.activePlayerGreen:setVisible(true)
            self.activePlayerPink:setVisible(false)
            self.activePlayerYellow:setVisible(false)
        elseif(self.playerIndex == 4) then
            player:setPlayerType(PLAYER_TYPE.PINK)
            self.activePlayerBeige:setVisible(false)
            self.activePlayerBlue:setVisible(false)
            self.activePlayerGreen:setVisible(false)
            self.activePlayerPink:setVisible(true)
            self.activePlayerYellow:setVisible(false)
        elseif(self.playerIndex == 5) then
            player:setPlayerType(PLAYER_TYPE.YELLOW)
            self.activePlayerBeige:setVisible(false)
            self.activePlayerBlue:setVisible(false)
            self.activePlayerGreen:setVisible(false)
            self.activePlayerPink:setVisible(false)
            self.activePlayerYellow:setVisible(true)
        end
        self.counter = 0
    end
    if(self.isVisible) then
        self.counter = self.counter + 1
        if self.counter == 100 then
            self.activePlayerBlue:setVisible(false)
            self.activePlayerBeige:setVisible(false)
            self.activePlayerGreen:setVisible(false)
            self.activePlayerPink:setVisible(false)
            self.activePlayerYellow:setVisible(false)
            self:setVisible(false)
            self.counter = 0
        end
    end
end