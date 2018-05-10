--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

--LIGHTING

light = {
    origin_x;
    origin_y;
    radius;
    intensity;
    brightness;
}

function newLight(origin_x, origin_y, radius, intensity, brightness)
    createLight(origin_x, origin_y, radius, intensity, brightness)
end
