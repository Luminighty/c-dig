local HEADER_GUARD = "TILE_H"
local file = "tile"
local header_file = file .. ".h"
local source_file = file .. ".c"

local function generate_enum(tiles)
	io.write("typedef enum {\n")
	io.write("\tTILE_NONE,\n")
	for _, tile in ipairs(tiles) do
		io.write(string.format("\tTILE_%s,\n", string.upper(tile.id)))
	end
	io.write("\tTILE_SIZE,\n")
	io.write("} Tile;\n")
end

local function header(tiles)
	HeaderGuardStart(HEADER_GUARD)

	PipeFile("./meta/tiles/template.h")
	io.write("\n")

	generate_enum(tiles)
	io.write("\n")
	io.write("extern TileData tile_data[TILE_SIZE];\n")
	io.write("\n")

	HeaderGuardEnd(HEADER_GUARD)
end

local function tile_flags(tile)
	local flags = {}
	if tile.is_solid then
		table.insert(flags, "TILE_FLAG_SOLID")
	end
	if tile.is_opaque then
		table.insert(flags, "TILE_FLAG_OPAQUE")
	end
	if tile.is_unbreakable then
		table.insert(flags, "TILE_FLAG_UNBREAKABLE")
	end
	if #flags == 0 then
		return "0"
	end
	return table.concat(flags, " | ")
end

local function source(tiles)
	SourceBegin(header_file)
	io.write("\n")
	io.write("TileData tile_data[TILE_SIZE] = {\n")

	for _, tile in ipairs(tiles) do
		io.write("\t[TILE_" .. string.upper(tile.id) .. "] = {\n")

		io.write("\t\t.texture = TEXTURE_" .. string.upper(tile.texture) .. ",\n")
		io.write("\t\t.hardness = " .. tile.hardness .. ",\n")
		io.write("\t\t.flags = " .. tile_flags(tile) .. ",\n")
		io.write("\t\t.layer = " .. tile.layer .. ",\n")

		io.write("\t},\n")
	end
	io.write("};\n")
end

return function()
	local tiles = GetTiles()

	io.output("./include/" .. header_file)
	header(tiles)
	io.close()

	io.output("./src/" .. source_file)
	source(tiles)
	io.close()
end
