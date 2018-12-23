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

function reloadLevel(arg)
    reloadLevelLua(arg)
    startLevel(arg)
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

function setAudioEnabled(arg)
    --
    --  Read the file
    --
    local f = io.open("lua/global_properties.lua", "r")
    local content = f:read("*all")
    f:close()


    --
    -- Edit the string
    --
    if(arg == true) then
        content = string.gsub(content, "\"audioDisabled\"", "\"audioEnabled\"")
    else
        content = string.gsub(content, "\"audioEnabled\"", "\"audioDisabled\"")
    end
    --
    -- Write it out
    --
    local f = io.open("lua/global_properties.lua", "w")
    f:write(content)
    f:close()

    setAudioEnabledLua(arg)
end

function setFullscreen(arg)
    --
    --  Read the file
    --
    local f = io.open("lua/global_properties.lua", "r")
    local content = f:read("*all")
    f:close()

    --
    -- Edit the string
    --
    if(arg == true) then
        content = string.gsub(content, "\"fullscreenDisabled\"", "\"fullscreenEnabled\"")
    else
        content = string.gsub(content, "\"fullscreenEnabled\"", "\"fullscreenDisabled\"")
    end
    --
    -- Write it out
    --
    local f = io.open("lua/global_properties.lua", "w")
    f:write(content)
    f:close()

    setFullscreenLua(arg)
end

function setVsync(arg)
    --
    --  Read the file
    --
    local f = io.open("lua/global_properties.lua", "r")
    local content = f:read("*all")
    f:close()

    --
    -- Edit the string
    --
    if(arg == true) then
        content = string.gsub(content, "\"vsyncDisabled\"", "\"vsyncEnabled\"")
    else
        content = string.gsub(content, "\"vsyncEnabled\"", "\"vsyncDisabled\"")
    end
    --
    -- Write it out
    --
    local f = io.open("lua/global_properties.lua", "w")
    f:write(content)
    f:close()

    setVsyncLua(arg)
end

if(audio == "audioEnabled") then
    setAudioEnabledLua(true)
else
    setAudioEnabledLua(false)
end

if(fullscreen == "fullscreenEnabled") then
    setFullscreenLua(true)
else
    setFullscreenLua(false)
end

if(vsync == "vsyncEnabled") then
    setVsyncLua(true)
else
    setVsyncLua(false)
end