require("meta.tiles.registry")
require("meta.minerals.registry")
require("meta.textures.registry")

TextureTemplate("TILE_ORE", "./assets/templates/tile_ore.png", {
	ColorGroup(0xad8c76, 0x7b6250),
})

function OreTile(id, props)
	assert(props.color ~= nil, "Ore requires a color! (id: " .. id .. ")")
	props.texture = props.texture or ("TILE_" .. id)
	GenerateTexture(props.texture, "TILE_ORE", { props.color })
	Tile(id, props)
end

----------------------
------- Assets -------
----------------------

-- NOTE: These might get renamed in the future
Mineral("dirt", { color = 0x623923 })
Mineral("stone", { color = 0x5e5e5e })
Mineral("iron", { color = 0xad8c760 })
Mineral("copper", { color = 0xee5511 })
Mineral("diamond", { color = 0xa4eeee })
Mineral("coal", { color = 0x0f0f0f })
Mineral("uranium", { color = 0x55b51b })
Mineral("gold", { color = 0xeebb22 })


OreTile("iron", {
	color = 0xad8c76,
	composition = {
		{ "stone", 15 },
		{ "iron", 5 },
	}
})
OreTile("diamond", {
	color = 0xa4eeee, hardness = 200,
	composition = {
		{ "stone", 18 },
		{ "diamond", 2 },
	}
})
OreTile("coal", {
	color = 0x2f2f2f,
	composition = {
		{ "stone", 10 },
		{ "coal", 10 },
	}
})
OreTile("uranium", {
	color = 0x55b51b,
	composition = {
		{ "stone", 15 },
		{ "uranium", 5 },
	}
})
OreTile("gold", {
	color = 0xeebb22,
	composition = {
		{ "stone", 12 },
		{ "gold", 8 },
	}
})
OreTile("copper", {
	color = 0xee5511,
	composition = {
		{ "stone", 10 },
		{ "copper", 10 },
	}
})

