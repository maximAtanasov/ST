--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

label = entity:new()
label.isVisible = true;
label.text = textObject:new()

function label:new(x, y, text, font, fontSize)
    local newLabel = newEntity(self, x, y)
    newLabel.text = textObject:new(x,y, fontSize, font)
    newLabel.text:setText(text)
    return newLabel
end

function label:hide()
    self:setVisible(false)
    self.label:setTextVisible(false)
end

function label:show()
    self:setVisible(true)
    self.label:setTextVisible(true)
end

function label:setText(text)
    self.text:setText(text)
end
