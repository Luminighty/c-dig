local C = require("cdef")
local Vec2 = require("linalg.vec2")
local Entity = require("cdef.Entity")
local Component = require("cdef.Component")

local query = Entity.query(Component.Position, Component.Chase)
local SPEED = 10.0

return function (world)
	for entity in Entity.foreach(world, query) do
		local position = C.entity_get_position(world, entity)
		local chase = C.entity_get_chase(world, entity)

		if not C.entity_is_alive(world, chase.target) then
			goto continue
		end
		local target_position = C.entity_get_position(world, chase.target)

		local pos = Vec2(position.x, position.y)
		local target = Vec2(target_position.x, target_position.y)
		local delta = target - pos
		if math.abs(delta.x) > SPEED or math.abs(delta.y) > SPEED then
			local move = delta:normalize():scale(SPEED)
			position[0] = pos + move
		end
	    ::continue::
	end
end

