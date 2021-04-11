-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

trigger = entity:new()
trigger.texWidth = 100
trigger.texHeight = 100
trigger.colX = 100
trigger.colY = 100
trigger.triggerCount = 0
trigger.isVisible = true
trigger.affectedByPhysics = false
trigger.collisionCallbacks = nil

function trigger:new(x, y, width, height)
    self.colX = width
    self.colY = height
    self.texWidth = width
    self.texHeight = height
    self = newEntity(self, x, y)
    self.collisionCallbacks = {}
    return self;
end

function trigger:onCollisionWith(object, callback)
    self.collisionCallbacks[object] = callback;
end

function trigger:update()
    for object, callback in pairs(self.collisionCallbacks) do
        if(self:overObject(object)) then
            callback()
            self.triggerCount = self.triggerCount + 1;
        end
    end
end

function trigger:getTriggerCount()
    return self.triggerCount
end

