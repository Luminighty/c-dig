function Build()
	require("assets.all")

	require("meta.math.generate")()
	require("meta.components.generate")()
	require("meta.textures.generate")()
	require("meta.tiles.generate")()
	require("meta.minerals.generate")()
end

Build()
