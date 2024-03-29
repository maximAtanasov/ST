-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

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
		o.font = font
		o.fontSize = fontSize
		o.x = x
		o.y = y
		createTextObject(x, y, o.text, font, fontSize);
    end
    return o
end

function textObject:new(x, y, fontSize, font)
	return newTextObject(self, x, y, fontSize, font)
end

function textObject:setText(text)
	self.text = text;
	setTextObjectText(self.ID, text)
end

function textObject:setFont(font)
	self.font = font;
	setTextObjectFont(self.ID, font.." "..self.fontSize)
end

function textObject:setX(x)
	self.x = x;
	setTextObjectX(self.ID, x)
end

function textObject:setY(y)
	self.y = y;
	setTextObjectY(self.ID, y)
end

function textObject:setFontSize(size)
	self.fontSize = size;
	setTextObjectFont(self.ID, self.font.." "..self.fontSize)
end

function textObject:setTextColor(r, g, b, a)
	self.r = r;
	self.g = b;
	self.b = b;
	self.a = a;
	setTextObjectColor(self.ID, r, g, b, a)
end

function textObject:setTextVisible(arg)
    setTextObjectVisible(self.ID, arg)
end
