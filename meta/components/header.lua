require("meta.utils")

local HEADER_GUARD = "COMPONENT_H"
local _, MODULE_PATH = ...


local function print_typedef(component)
	if component.extern then
		return
	end
	-- io.write("// ", component.storage, "\n")
	io.write("typedef ")
	if component.storage == Storage.Flag then
		io.write("struct {}")
	elseif component.fields ~= nil then
		io.write("struct {\n")
		for _, pair in ipairs(component.fields) do
			local field = pair[1]
			local type = pair[2]
			io.write("\t", type, " ", field, ";\n")
		end
		io.write("}")
	elseif component.actual_type then
		io.write(component.actual_type)
	else
		assert(0, "Unknown component type " .. component.class_name)
	end
	io.write(" ", component.class_name, ";\n\n")
end

function WriteComponentEnum(components)
	io.write("typedef enum {\n")
	for _, value in ipairs(components) do
		io.write("\tCOMPONENT_", string.upper(value.class_name), ",\n")
	end
	io.write("\tCOMPONENT__SIZE,\n")
	io.write("} ComponentType;\n\n")
end

local function append_header_template()
	PipeFile("./meta/components/template_component.h")
end

local function generate_xmacro(components)
	io.write("#define COMPONENTS \\")

	local formats = {
		[Storage.Dense] = "DENSE(%s, %s, COMPONENT_%s)",
		[Storage.Flag] = "FLAG(%s, %s, COMPONENT_%s)",
	}

	for i, component in ipairs(components) do
		if i > 1 then
			io.write("\\")
		end
		local lower = string.lower(component.class_name)
		local upper = string.upper(component.class_name)
		local name = component.class_name
		local format = formats[component.storage]
		io.write("\n ")
		io.write(string.format(format, name, lower, upper))
	end
	io.write("\n\n")
end

function WriteComponentBitmap(skip_macros)
	local slot_size = 64

	if not skip_macros then
		io.write(("#define COMPONENTBITMAP_SLOTSIZE %d\n"):format(slot_size))
		io.write("#define COMPONENT_TO_SLOTBIT(component) (1ULL << ((component) % COMPONENTBITMAP_SLOTSIZE))\n")
	end
	io.write("typedef struct {\n")
	io.write(("\tuint%d_t bytes[(COMPONENT__SIZE / %d) + 1];\n"):format(slot_size, slot_size))
	io.write("} ComponentBitmap;\n")
	io.write("\n")
end

function WriteComponentAdd(component)
	local name = component.class_name
	local fn = string.lower(component.class_name)
	if component.storage == Storage.Dense then
		local format = "%s *entity_add_%s(struct world* _world, union entity _entity, %s data)"
		io.write(string.format(format, name, fn, name))
	elseif component.storage == Storage.Flag then
		local format = "void entity_add_%s(struct world* _world, union entity _entity)"
		io.write(string.format(format, fn))
	end
end

function WriteComponentRemove(component)
	local fn = string.lower(component.class_name)
	local format = "void entity_remove_%s(struct world* _world, union entity _entity)"
	io.write(string.format(format, fn))
end

function WriteComponentGet(component)
	local comp = string.lower(component.class_name)
	-- local format = "#define entity_get_%s(world, entity) (entity_get_component(world, entity, %s))\n"
	-- io.write(string.format(format, comp, comp))
	-- local comp = string.lower(component.class_name)
	local format = "%s *entity_get_%s(struct world* _world, union entity _entity)"
	-- format = "DEFINE_COMPONENT_GETTER(%s)"
	io.write(string.format(format, component.class_name, comp))
end

function WriteComponentTypeDefs(components)
	for _, value in ipairs(components) do
		print_typedef(value)
	end
end


function WriteComponentFunctionDefs(components)
	for _, value in ipairs(components) do
		if value.storage ~= "FLAG" then
			WriteComponentGet(value)
			io.write(";\n")
		end
		WriteComponentAdd(value)
		io.write(";\n")
		WriteComponentRemove(value)
		io.write(";\n\n")
	end
end

function GenerateHeaders(components, includes, outfile)
	io.output(outfile)
	HeaderGuardStart(HEADER_GUARD, MODULE_PATH)

	io.write("#include <stdint.h>\n")
	io.write("#include <stdbool.h>\n")

	for _, include in ipairs(includes) do
		io.write('#include "', include, '"\n')
	end
	io.write("\n")

	WriteComponentEnum(components)
	append_header_template()
	WriteComponentTypeDefs(components)
	WriteComponentFunctionDefs(components)
	generate_xmacro(components)
	WriteComponentBitmap()

	HeaderGuardEnd(HEADER_GUARD)

	io.close()
	io.output(io.stdout)
end
