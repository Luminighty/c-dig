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
	texture = TEXTURE_NONE,
	is_unbreakable = true,
})
SimpleTile("air", {
	texture = "tile_stone",
	is_solid = false,
	is_opaque = false,
	hardness = 0,
	layer = RenderLayer.BACKGROUND,
})
SimpleTile("dirt", {
	composition = {
		{ "dirt", 5 }
	}
})
SimpleTile("grass", {
	composition = {
		{ "dirt", 5 }
	}
})
SimpleTile("moss", {
	composition = {
		{ "dirt", 5 }
	}
})
SimpleTile("stone", {
	composition = {
		{ "stone", 5 }
	}
})
SimpleTile("wood", {})
