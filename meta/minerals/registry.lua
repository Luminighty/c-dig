require("meta.utils")

local minerals = {}
local mineral_ids = {}

local reserved = { none = true, size = true }
function GetMinerals()
	return Values(minerals)
end

function Mineral(id, props)
	assert(reserved[string.lower(id)] == nil, "Mineral '" .. id .. "' is reserved.")
	assert(mineral_ids[id] == nil, "Mineral '" .. id .. "' already defined.")
	mineral_ids[id] = true
	table.insert(minerals, {
		id = id,
		color = props.color
	})
end

