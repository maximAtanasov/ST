--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com


currentID = 0 --MUST start at 0
currentVolume = 100;
math.randomseed( os.time() )

function newID()
    local temp = currentID
    currentID = currentID + 1;
    return temp
end

--cleans up current level and start the specifed one
function startLevel(arg)
    startLevelLua(arg)
    currentID = 0
    error() --a dirty trick, but works
end
