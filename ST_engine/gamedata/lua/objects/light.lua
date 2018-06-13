--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

--LIGHTING
function newLightID()
    local temp = currentLightID
    currentLightID = currentLightID + 1;
    return temp
end

light = {
    ID;
    origin_x;
    origin_y;
    radius = 0;
    intensity = 0;
    brightness = 0;
}

function newLight(self, origin_x, origin_y, radius, intensity, brightness)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    if(origin_x ~= nil and origin_y ~= nil) then
        o.ID = newLightID()
        createLight(o.ID, origin_x, origin_y, radius, intensity, brightness)
    end
    return o
end


--Creates a new light object and returns it
function light:new(origin_x, origin_y, radius, intensity, brightness)
    return newLight(self, origin_x, origin_y, radius, intensity, brightness)
end

--Simple getters and setters

function light:setOriginX(origin_x)
    setLightOriginX(self.ID, origin_x)
end

function light:getOriginX()
    return getLightOriginX(self.ID)
end

function light:setOriginY(origin_y)
    setLightOriginY(self.ID, origin_y)
end

function light:getOriginY()
    return getLightOriginY(self.ID)
end

function light:setBrightness(brightness)
    setLightBrightness(self.ID, brightness)
end

function light:getBrightness()
    return getLightBrightness(self.ID)
end

function light:setRadius(radius)
    setLightRadius(self.ID, radius)
end

function light:getRadius()
    return getLightRadius(self.ID)
end

function light:setIntensity(intensity)
    setLightIntensity(self.ID, intensity)
end

function light:getIntensity()
    return getLightIntensity(self.ID);
end

