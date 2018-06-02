--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

--TEXT

function newTextID()
    local temp = currentTextID
    currentTextID = currentTextID + 1;
    return temp
end

textObject = {
	ID;
	x;
	y;
	text = " ";
	font = " ";
	fontSize = 40;
	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

function newTextObject(self, x, y, fontSize, font)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    if(x ~= nil and y ~= nil) then
        o.ID = newTextID()
		createTextObject(o.ID, x, y, o.text, font, fontSize);
    end
    return o
end

function textObject:new(x, y, fontSize, font)
	return newTextObject(self, x, y, fontSize, font)
end

function textObject:setText(text)
	setTextObjectText(self.ID, text)
end

function textObject:setFont(font)
	setTextObjectFont(self.ID, font)
end

function textObject:setFontSize(size)
	setTextObjectFontSize(self.ID, size)
end

function textObject:setTextColor(r, g, b, a)
	setTextObjectColor(self.ID, r, g, b, a)
end

function textObject:setTextVisible(arg)
    setTextObjectVisible(self.ID, arg)
end
