require("meta.tiles.registry")

local HEADER_GUARD = "TILE_H"
local file = "tile"
local header_file = file .. ".h"
local source_file = file .. ".c"

local function generate_enum(tiles)
	io.write("typedef enum {\n")
	io.write("\tTILE__NONE,\n")
	for _, tile in ipairs(tiles) do
		io.write(string.format("\tTILE_%s,\n", string.upper(tile.id)))
	end
	io.write("\tTILE__SIZE,\n")
	io.write("} Tile;\n")
end

local function header(tiles)
	HeaderGuardStart(HEADER_GUARD)

	PipeFile("./meta/tiles/template.h")
	io.write("\n")

	generate_enum(tiles)
	io.write("\n")
	io.write("extern TileData tile_data[TILE__SIZE];\n")
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

local function tile_composition(tile)
	local items = {}
	for _, comp in ipairs(tile.composition) do
		local mineral = "MINERAL_" .. string.upper(comp[1])
		local amount = comp[2]
		local fmt = "{ .mineral = %s, .amount = %d }"
		table.insert(items, fmt:format(mineral, amount))
	end
	if #items == 0 then return "{}" end
	return "{\n\t\t\t" .. table.concat(items, ",\n\t\t\t") .. "\n\t\t}"
end

local function source(tiles)
	SourceBegin(header_file)
	io.write("\n")
	io.write("TileData tile_data[TILE__SIZE] = {\n")

	for _, tile in ipairs(tiles) do
		io.write("\t[TILE_" .. string.upper(tile.id) .. "] = {\n")

		io.write("\t\t.texture = TEXTURE_" .. string.upper(tile.texture) .. ",\n")
		io.write("\t\t.hardness = " .. tile.hardness .. ",\n")
		io.write("\t\t.flags = " .. tile_flags(tile) .. ",\n")
		io.write("\t\t.layer = " .. tile.layer .. ",\n")
		io.write("\t\t.composition = " .. tile_composition(tile) .. ",\n")

		io.write("\t},\n")
	end
	io.write("};\n")
end

return function()
	local tiles = GetTiles()

	io.output("./include/" .. header_file)
	header(tiles)
	io.close()

	io.output("./core/" .. source_file)
	source(tiles)
	io.close()
end
