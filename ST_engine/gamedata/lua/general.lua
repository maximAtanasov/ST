-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: atanasovmaksim1@gmail.com

local io = require "io"

currentID = 0 --MUST start at 0
currentTextID = 0 --MUST start at 0
currentLightID = 0 --MUST start at 0

currentVolume = 100
math.randomseed(os.time())

--cleans up current level and start the specifed one
function startLevel(arg)
    startLevelLua(arg)
    currentID = 0
    currentLightID = 0
    currentTextID = 0
    error() --a dirty trick, but it works
end

function exit()
    endGame()
end

function setLanguage(language_string)
    --
    --  Read the file
    --
    local f = io.open("lua/global_properties.lua", "r")
    local content = f:read("*all")
    f:close()


    --
    -- Edit the string
    --
    content = string.gsub(content, language, language_string)
    --
    -- Write it out
    --
    local f = io.open("lua/global_properties.lua", "w")
    f:write(content)
    f:close()
end