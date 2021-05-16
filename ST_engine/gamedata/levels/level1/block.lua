-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

BLOCK_TYPE = {
    GRASS = "ground_grass1.png",
    GRASS_ROUNDED = "ground_grass2.png",
    GRASS_ROUNDED_LEFT = "ground_grass3.png",
    GRASS_EDGE_WATER = "ground_edge_water.png",
}

function makeBlock(x, y, blockType)
    local block = entity:new()
    block.texture = blockType
    if blockType == BLOCK_TYPE.GRASS_EDGE_WATER then
        block.texWidth = 128
        block.colX = 128
    else
        block.texWidth = 512
        block.colX = 512
    end
    block.texHeight = 128
    block.colY = 128
    block.isVisible = true
    block.affectedByPhysics = true
    return newEntity(block, x, y)
end