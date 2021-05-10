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
    local decal = entity:new(x, y)
    decal.texHeight = 128
    decal.texWidth = 128
    decal:setTexW(128)
    decal:setTexH(128)
    decal:setVisible(true)
    decal:setTexture(decalType)
end