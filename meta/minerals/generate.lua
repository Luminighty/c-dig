require("meta.minerals.registry")

local HEADER_GUARD = "MINERAL_H"
local file = "mineral"
local header_file = file .. ".h"
local source_file = file .. ".c"

local data_config = "MineralData mineral_data[MINERAL__SIZE]";

local function generate_enum(minerals)
	io.write("typedef enum {\n")
	io.write("\tMINERAL__NONE,\n")
	for _, mineral in ipairs(minerals) do
		io.write(string.format("\tMINERAL_%s,\n", string.upper(mineral.id)))
	end
	io.write("\tMINERAL__SIZE,\n")
	io.write("} Mineral;\n")
end

local function header(minerals)
	HeaderGuardStart(HEADER_GUARD)

	PipeFile("./meta/minerals/template.h")
	io.write("\n")

	generate_enum(minerals)
	io.write("\n")
	io.write("extern " .. data_config .. ";\n")
	io.write("\n")

	io.write("\n\n")
	HeaderGuardEnd(HEADER_GUARD)
end

local function source(minerals)
	SourceBegin(header_file)
	io.write("\n")
	io.write(data_config .. " = {\n")

	for _, mineral in ipairs(minerals) do
		io.write("\t[MINERAL_" .. string.upper(mineral.id) .. "] = {\n")
		io.write("\t\t.color = " .. string.format("0x%x", mineral.color) .. ",\n")
		io.write("\t},\n")
	end
	io.write("};\n")
end

return function()
	local minerals = GetMinerals()

	io.output("./include/" .. header_file)
	header(minerals)
	io.close()

	io.output("./src/" .. source_file)
	source(minerals)
	io.close()
end
