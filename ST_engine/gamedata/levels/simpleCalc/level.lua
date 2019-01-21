-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

setDarkness(0)
setWindowResolution(432, 768)
setFullscreen(false)
setInternalResolution(1080, 1920)
setBackgroundColor(255, 255, 255, 255);
showMouseCursor()
stopMusic();
stopAllSounds();


resultLabel = label:new(100, 400, "", "font1.ttf", 140);
resultLabel:setTextColor(0, 0, 0,0 );
equalsWasPressed = false;

lineLabel1 = label:new(0, 620, "_________________________________________________", "font1.ttf", 140);
lineLabel1:setTextColor(0, 0, 0,0 );


button_calc = button:new(0, 0, " ", " ", 0);
button_calc:setMarginX(170)
button_calc:setMarginY(170)
button_calc:setMarginOffsetX(-50)
button_calc:setMarginOffsetY(20)
button_calc:setClickKey("MOUSE1")

function button_calc:onClick()
    if(self:getText() == "=") then
        equalsWasPressed = true;
        eval = load('return '..resultLabel:getText());
        resultLabel:setText(tostring(eval()))
    else
        if(equalsWasPressed) then
            resultLabel:setText("")
            equalsWasPressed = false
        end
        resultLabel:setText(resultLabel:getText()..self:getText());
    end
end

function button_calc:onHover()
    self:setTextColor(255, 50, 50, 200);
end

function button_calc:onNothing()
    self:setTextColor(0, 0, 0, 255);
end


--ROW 1
digit7 = button_calc:new(100, 800, "7", "font1.ttf", 140)
digit8 = button_calc:new(100+250, 800, "8", "font1.ttf", 140)
digit9 = button_calc:new(100+250+250, 800, "9", "font1.ttf", 140)
divide = button_calc:new(100+250+250+250, 800, "/", "font1.ttf", 140)

--ROW 2
digit4 = button_calc:new(100, 1100, "4", "font1.ttf", 140)
digit5 = button_calc:new(100+250, 1100, "5", "font1.ttf", 140)
digit6 = button_calc:new(100+250+250, 1100, "6", "font1.ttf", 140)
multiply = button_calc:new(100+250+250+250, 1100, "*", "font1.ttf", 140)

--ROW 3
digit1 = button_calc:new(100, 1400, "1", "font1.ttf", 140)
digit2 = button_calc:new(100+250, 1400, "2", "font1.ttf", 140)
digit3 = button_calc:new(100+250+250, 1400, "3", "font1.ttf", 140)
minus = button_calc:new(100+250+250+250, 1400, "-", "font1.ttf", 140)

--ROW 4
digit0 = button_calc:new(100, 1700, "0", "font1.ttf", 140)
point = button_calc:new(100+250, 1700, ".", "font1.ttf", 140)
equals = button_calc:new(100+250+250, 1700, "=", "font1.ttf", 140)
plus = button_calc:new(100+250+250+250, 1700, "+", "font1.ttf", 140)

function loop()
    digit7:update()
    digit8:update()
    digit9:update()
    divide:update()

    digit4:update()
    digit5:update()
    digit6:update()
    multiply:update()

    digit1:update()
    digit2:update()
    digit3:update()
    minus:update()

    digit0:update()
    point:update()
    equals:update()
    plus:update()
end