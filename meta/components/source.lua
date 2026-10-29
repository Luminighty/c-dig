require("meta.utils")
local _, MODULE_PATH = ...

local function append_source_template()
	PipeFile("./meta/components/template_component.c")
end

function WriteComponentAddSrc(component)
	local enum = string.upper(component.class_name)
	local field = string.lower(component.class_name)
	local storage = string.upper(component.storage)
	local format = " {\tENTITY_ADD_%s(_world, _entity, %s, data, COMPONENT_%s); }\n"
	io.write(string.format(format, storage, field, enum))
end

function WriteComponentRemoveSrc(component)
	local enum = string.upper(component.class_name)
	local format = " {\tENTITY_REMOVE(_world, _entity, COMPONENT_%s); }\n"
	io.write(string.format(format, enum))
end

function WriteComponentGetSrc(component)
	if component.storage == "FLAG" then return end
	local field = string.lower(component.class_name)
	local format = " {\treturn entity_get_component(_world, _entity, %s); }\n"
	-- format = "DEFINE_COMPONENT_GETTER(%s)"
	io.write(string.format(format, field))
end

function GenerateSource(components, outfile)
	io.output(outfile)
	SourceBegin("components.h", MODULE_PATH)

	io.write('#include "components.h"\n\n')
	io.write('#include "entity.h"\n\n')
	append_source_template()
	io.write("\n")
	for _, value in ipairs(components) do
		WriteComponentAdd(value)
		WriteComponentAddSrc(value)
		WriteComponentRemove(value)
		WriteComponentRemoveSrc(value)
		if value.storage ~= "FLAG" then
			WriteComponentGet(value)
			WriteComponentGetSrc(value)
		end
		io.write("\n")
	end

	io.close()
	io.output(io.stdout)
end
