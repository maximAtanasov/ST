-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

--This file is used to bootstrap the first level of the game

local level_name = "intro_splash"

loadLevel(level_name)

function loop()
    startLevel(level_name)
end
