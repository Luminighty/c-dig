require("meta.tiles.registry")
require("meta.textures.registry")

TextureTemplate("GEM", "./assets/templates/tile_gem.png", {
	ColorGroup(0xbf1f1f, 0x880b0b),
})

function Gem(id, props)
	assert(props.color ~= nil, "Gem requires a color! (id: " .. id .. ")")
	props.texture = props.texture or ("TILE_" .. id)
	GenerateTexture(props.texture, "GEM", { props.color })
	Tile(id, props)
end

----------------------
------- Assets -------
----------------------

Gem("ruby", { color = 0xbf1f1f })
Gem("emerald", { color = 0x8f1fbf })
Gem("amethyst", { color = 0x1fbf22 })
