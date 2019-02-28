-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

platform = entity:new()
platform.texture = "obstacle_1.png"
platform.texWidth = 400
platform.texHeight = 150
platform.colX = 400
platform.colY = 132
platform.isVisible = true
platform.affectedByPhysics = true
platform.height = 0;

function platform:update ()
    self:setY(self.height)
end
