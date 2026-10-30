

function Build()
	require("assets.all")

	require("meta.math")()
	require("meta.components")()
	require("meta.textures")()
	require("meta.tiles")()
	require("meta.minerals")()
	require("meta.ffi")()
end

Build()
