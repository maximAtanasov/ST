--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

button = object:new()
button.soundPlayed = 0
button.isVisible = true;
button.text = textObject:new()
button.colX = 0
button.colY = 0
button.clickKey = ""

function createButton(x, y, text, font, fontSize)
    local newButton = button:new(x, y)
    newButton.text = textObject:new(x,y, fontSize, font)
    newButton.text:setText(text)
    return newButton
end

function button:setClickKey(x)
    self.clickKey = x
end

function button:setMarginX(x)
    self:setTexW(x)
    self.colX = x
    self:setCollision(0,0,x,self.colY)
end

function button:setMarginY(y)
    self:setTexH(y)
    self.colY = y
    self:setCollision(0,0, self.colX, y)
end

function button:onClick()

end

function button:onHover()

end

function button:onNothing()

end

function button:update()
    if mouseOver(self) then
        self:onHover()
        if keyPressed(self.clickKey) then
            self:onClick()
        end
    else
        self:onNothing()
    end
end

function button:hide()
    self:setVisible(false)
    self.text:setTextVisible(false)
end

function button:show()
    self:setVisible(true)
    self.text:setTextVisible(true)
end

function button:setText(text)
    self.text:setText(text)
end
