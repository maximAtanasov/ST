-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

crate = entity:new()
crate.texture = "crate1.png"
crate.texWidth = 128
crate.texHeight = 128
crate.colX = 128
crate.colY = 128
crate.isVisible = true
crate.affectedByPhysics = true

function crate:new(x, y)
    local random = math.random
    self.texture = 'crate'..random(2, 4)..'.png'
    self = newEntity(self, x, y)
    return self;
end