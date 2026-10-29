package.path = package.path .. ";./lua/?/init.lua;./lua/?.lua"

require("cdef")
local Systems = require("systems")


function OnInit()

end

function OnUnload()

end

function OnUpdate()
	Systems.Update(World)
end

