local C = require("cdef")
local Vec2 = require("linalg.vec2")
local Entity = require("cdef.Entity")
local Component = require("cdef.Component")

local query = Entity.query(Component.Position, Component.RandomWalk)
return function (world)
	for entity in Entity.foreach(world, query) do
		local position = C.entity_get_position(world, entity)
		local pos = Vec2(position.x, position.y)
		local walk = C.entity_get_randomwalk(world, entity)

		local delta = walk.target - pos
		if math.abs(delta.x) > 1.0 or math.abs(delta.y) > 1.0 then
			local move = delta:normalize():scale(0.05)
			position[0] = pos + move
		else
			local target_delta = Vec2(
				math.random(-30, 30),
				math.random(-30, 30)
			)
			local new_pos = pos + target_delta
			if new_pos.x < 0   then new_pos.x = 30  end
			if new_pos.x > 500 then new_pos.x = 470 end
			if new_pos.y < 0   then new_pos.y = 30  end
			if new_pos.y > 500 then new_pos.y = 470 end
			walk.target = new_pos
		end
	end
end

