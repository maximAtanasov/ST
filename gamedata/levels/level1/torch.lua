torch = object:new()
torch.texture = "torch.png"
torch.texWidth = 630
torch.texHeight = 140
torch.colX = 0;
torch.colY = 0;
torch.isVisible = true
torch.animationNum = 1
torch.spriteNum = 9
torch.affectedByPhysics = false

function torch:play_animation()
    self:playAnimation(1)
end
