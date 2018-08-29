-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com

checkbox = entity:new()
checkbox.isVisible = true
checkbox.checked = false
checkbox.soundPlayed = 0
checkbox.clickKey = ""
checkbox.colX = 0
checkbox.colY = 0

function checkbox:new(x, y)
    local newCheckbox = newEntity(self, x, y) 
    newCheckbox:setTexture("checkbox-unchecked.webp")
    newCheckbox:setTexW(50)
    newCheckbox:setTexH(50)
    newCheckbox.colX = 50
    newCheckbox.colY = 50
    newCheckbox:setCollision(0, 0, 50 ,50)
    return newCheckbox
end

function checkbox:hide()
    self:setVisible(false)
end

function checkbox:show()
    self:setVisible(true)
end

function checkbox:check()
    self.checked = true
    self:setTexture("checkbox-checked.webp")
end

function checkbox:uncheck()
    self.checked = false
    self:setTexture("checkbox-unchecked.webp")
end

function checkbox:toggle()
    if self.checked then
        self:uncheck()
    else
        self:check()
    end
end

function checkbox:update()
    if mouseOver(self) then
        self:onHover()
        if keyPressed(self.clickKey) then
            self:toggle()
            self:onClick()
        end
    else
        self:onNothing()
    end
end

function checkbox:setClickKey(x)
    self.clickKey = x
end

function checkbox:onClick()

end

function checkbox:onHover()

end

function checkbox:onNothing()

end

function checkbox:setMarginX(x)
    self:setTexW(x)
    self.colX = x
    self:setCollision(0, 0, x, self.colY)
end

function checkbox:setMarginY(y)
    self:setTexH(y)
    self.colY = y
    self:setCollision(0, 0, self.colX, y)
end