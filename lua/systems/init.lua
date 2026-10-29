local RandomWalk = require("systems.randomwalk")
local Chase = require("systems.chase")

local Systems = {}

function Systems.Update(world)
	RandomWalk(world)
	Chase(world)
end

return Systems
