--Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
--You may not use, distribute or modify this code.
--This code is proprietary and belongs to the "slavicTales"
--project. See LICENCE.txt in the root directory of the project.
--
-- E-mail: atanasovmaksim1@gmail.com

torch = entity:new()
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
