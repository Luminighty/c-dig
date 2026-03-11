require("meta.tiles.registry")
require("meta.textures.registry")

function SimpleTile(id, props)
	props.texture = string.upper(props.texture or ("TILE_" .. id))

	Tile(id, props)
end

----------------------
------- Assets -------
----------------------

SimpleTile("bedrock", {
	texture = "none",
	is_unbreakable = true,
})
SimpleTile("air", {
	texture = "tile_stone",
	is_solid = false,
	is_opaque = false,
	layer = RenderLayer.BACKGROUND,
})
SimpleTile("dirt", {})
SimpleTile("grass", {})
SimpleTile("moss", {})
SimpleTile("stone", {})
SimpleTile("wood", {})
