require("meta.utils")
require("meta.components.header")
local _, MODULE_PATH = ...

local extra_cdefs = [[
	ComponentBitmap _componentbitmap_create(ComponentType c, ...);
	void componentbitmap_add(ComponentBitmap *bitmap, ComponentType c);
]]

function GenerateComponentFFI(components, outfile)
	io.output(outfile)
	FileMetaComment(MODULE_PATH, "--")
	WriteLuaRequire("ffi")
	io.write("\n")

	io.write("ffi.cdef[[\n")
	io.write("struct world;\n")
	io.write("union entity;\n")
	io.write("\n")
	WriteComponentEnum(components)
	WriteComponentTypeDefs(components)
	WriteComponentFunctionDefs(components)
	WriteComponentBitmap(true)
	io.write(extra_cdefs)
	io.write("]]\n\n")

	io.write("local Component = {\n")
	for _, value in ipairs(components) do
		io.write(("\t%s = ffi.C.COMPONENT_%s,\n"):format(value.class_name, string.upper(value.class_name)))
	end
	io.write("}\n")
	io.write("return Component")
end


function GeneratePrefabFFI(outfile)
	io.output(outfile)
	FileMetaComment(MODULE_PATH, "--")
	WriteLuaRequire("ffi")
	io.write("\n")

	io.write("ffi.cdef[[\n")
		ReadHeaderIntoFFI("./include/entity_prefabs.h")
	io.write("]]\n\n")
end

