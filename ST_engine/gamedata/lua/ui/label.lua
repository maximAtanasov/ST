-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com

label = entity:new()
label.isVisible = true;
label.text = nil;

function label:new(x, y, text, font, fontSize)
    local newLabel = newEntity(self, x, y)
    newLabel.text = textObject:new(x,y, fontSize, font)
    newLabel.text:setText(text)
    return newLabel
end

function label:hide()
    self:setVisible(false)
    self.text:setTextVisible(false)
end

function label:show()
    self:setVisible(true)
    self.text:setTextVisible(true)
end

function label:setText(text)
    self.text:setText(text)
end

function label:setFont(font)
    self.text:setFont(font)
end

function label:setFontSize(size)
    self.text:setFontSize(size)
end

function label:setTextColor(r, g, b, a)
    self.text:setTextColor(r, g, b, a)
end