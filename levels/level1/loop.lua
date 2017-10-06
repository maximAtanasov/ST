--code is run in a loop inside the main executable

if(pause == 0) then
    if keyPressed("PAUSE") then
        pauseGame()
    elseif keyPressed("COLLISIONS") then
        toggleCollisions()
    elseif keyPressed("R") then
        startLevel("level1")
    elseif keyPressed("QUIT") then
        endGame()
    end
    player1:update()
    coin1:update()
    coin2:update()
    coin3:update()
    coin4:update()
    coin5:update()
    coin6:update()
    crystal_green1:update()
    coin7:update()
    coin8:update()
    coin9:update()
    coin10:update()
    coin11:update()
    coin12:update()
    wolf1:update()
    if keyPressed("CONSOLE") then
        toggleConsole()
    end
    centreCamera(player1.ID)    
elseif(pause == 1) then
    button_continue:update()
    button_exit:update()
    if keyPressed("PAUSE") then
        unpauseGame()
    end
end
