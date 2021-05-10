-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

DECAL_TYPE = {
    GRASS = "grass.png",
    ROCK = "rock.png",
    FENCE = "fence.png",
    FENCE_BROKEN = "fenceBroken.png",
    BUSH = "bush.png",
    MUSHROOM_BROWN = "mushroomBrown.png",
    MUSHROOM_RED = "mushroomRed.png"
}

function makeDecal(x, y, decalType)
    local decal = {}
    decal.texHeight = 128
    decal.texWidth = 128
    decal.isVisible = true
    decal.texture = decalType
    return newEntity(decal, x, y)
end
