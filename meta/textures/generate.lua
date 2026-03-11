require("meta.utils")
require("meta.textures.registry")

local HEADER_GUARD = "TEXTURE_REGISTRY"
local header_file = "texture_registry.h"

local function write_textures(generator, textures)
	generator:write(#textures)
	generator:write("\n")
	for _, texture in ipairs(textures) do
		local file_name = string.gsub(string.lower(texture.id), "_", "/")
		local path = string.format("./assets/textures/%s.png", file_name)
		generator:write(path)
		generator:write("\n")
		for _, color in ipairs(texture.colors) do
			generator:write(color)
			generator:write("\n")
		end
	end
end

local function write_template_header(generator, template)
	generator:write(template.file)
	generator:write("\n")
	generator:write(#template.colors)
	generator:write("\n")
	for _, group in ipairs(template.colors) do
		generator:write(#group)
		for _, color in ipairs(group) do
			generator:write(" ")
			generator:write(color)
		end
	end
	generator:write("\n")
end

local function generate_template_textures(templates)
	local generator = assert(io.popen("./meta/textures/generator build", "w"))
	-- io.output(generator)
	generator:write(#templates)
	generator:write("\n")
	for _, template in pairs(templates) do
		write_template_header(generator, template.template)
		write_textures(generator, template.textures)
		generator:flush()
	end
	generator:flush()
	generator:close()
end

local function generate_textures_header(textures)
	io.output("./include/" .. header_file)
	HeaderGuardStart(HEADER_GUARD)

	io.write("#define REGISTRY_TEXTURES \\\n")
	for _, texture in ipairs(textures) do
		local format = '\tTEXTURE(%s, "%s")\\\n'
		io.write(string.format(format, texture.id, texture.path))
	end
	io.write("\n")
	io.write("\n")

	io.write("typedef enum {\n")
	io.write("\tTEXTURE_NONE,\n")
	io.write("\n")
	io.write("\t#define TEXTURE(id, _) TEXTURE_##id,\n")
	io.write("\tREGISTRY_TEXTURES\n")
	io.write("\t#undef TEXTURE\n")
	io.write("\n")
	io.write("\tTEXTURE_SIZE,\n")
	io.write("} TextureId;\n")
	io.write("\n")

	HeaderGuardEnd(HEADER_GUARD)
	io.close()
end

local function path_to_id(path)
	path = string.gsub(path, "./assets/textures/", "")
	path = string.gsub(path, ".png", "")
	path = string.gsub(path, "/", "_")
	return string.upper(path)
end

local function generate_header() end

return function()
	local templates = Values(GetTexturesGroupByTemplates())
	-- Dump(templates)
	-- io.write("\n")
	generate_template_textures(templates)
	-- io.output("./include/" .. header_file)

	local textures = {}
	for texture in FindTexturesIter() do
		table.insert(textures, {
			path = texture,
			id = path_to_id(texture),
		})
	end
	generate_textures_header(textures)
end
