-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

button = entity:new()
button.soundPlayed = 0
button.isVisible = true
button.text = nil
button.colX = 0
button.colY = 0
button.clickKey = ""
button.affectedByPhysics = false

function button:new(x, y, text, font, fontSize)
    local newButton = newEntity(self, x, y)
    newButton.text = textObject:new(x, y, fontSize, font)
    newButton.text:setText(text)
    return newButton
end

function button:setClickKey(x)
    self.clickKey = x
end

function button:setMarginX(x)
    self:setTexW(x)
    self.colX = x
    self:setCollision(self.offsetColX, self.offsetColY, x, self.colY)
end

function button:setMarginY(y)
    self:setTexH(y)
    self.colY = y
    self:setCollision(self.offsetColX, self.offsetColY, self.colX, y)
end

function button:setMarginOffsetY(y)
    self.offsetColY = y;
    self:setCollision(self.offsetColX, y, self.colX, self.colY)
end

function button:setMarginOffsetX(x)
    self.offsetColX = x
    self:setCollision(x, self.offsetColY, self.colX, self.colY)
end

function button:onClick()

end

function button:onHover()

end

function button:onNothing()

end

function button:update()
    if mouseOver(self) then
        if keyPressed(self.clickKey) then
            self:onClick()
        else
            self:onHover()
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

function button:getText()
    return self.text.text
end

function button:setTextColor(r,g,b,a)
    self.text:setTextColor(r,g,b,a)
end