require("meta.utils")

local tiles = {}
local tile_ids = {}

local reserved = { none = true, size = true }
function GetTiles()
	return Values(tiles)
end

RenderLayer = {
	BACKGROUND = "RENDER_LAYER_BACKGROUND",
	NORMAL = "RENDER_LAYER_NORMAL",
	FOREGROUND = "RENDER_LAYER_FOREGROUND",
}

function Tile(id, props)
	assert(reserved[string.lower(id)] == nil, "Tile '" .. id .. "' is a reserved keyword.")
	assert(tile_ids[id] == nil, "Tile '" .. id .. "' already defined.")
	tile_ids[id] = true
	props = props or {}
	table.insert(tiles, {
		id = id,
		texture = props.texture or ("TILE_" .. id),
		layer = props.layer or RenderLayer.NORMAL,
		hardness = props.hardness or 16,
		composition = props.composition or {},
		is_solid = Default(props.is_solid, true),
		is_opaque = Default(props.is_opaque, true),
		is_unbreakable = Default(props.is_unbreakable, false),
	})
end

