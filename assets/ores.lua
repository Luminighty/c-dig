require("meta.tiles.registry")
require("meta.textures.registry")

TextureTemplate("ORE", "./assets/templates/tile_ore.png", {
	ColorGroup(0xad8c76, 0x7b6250),
})

function Ore(id, props)
	assert(props.color ~= nil, "Ore requires a color! (id: " .. id .. ")")
	props.texture = props.texture or ("TILE_" .. id)
	GenerateTexture(props.texture, "ORE", { props.color })
	Tile(id, props)
end

----------------------
------- Assets -------
----------------------

Ore("iron", { color = 0xad8c76 })
Ore("diamond", { color = 0xa4eeee, hardness = 200 })
Ore("coal", { color = 0x2f2f2f })
Ore("uranium", { color = 0x55b51b })
