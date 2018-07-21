-- This file is part of the "slavicTales" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
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
