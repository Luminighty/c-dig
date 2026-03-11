-- Allows the definition of shaded colors using only one main color
--   For example: red, light_red, dark_red
--   During generation, you would only have to pass "green" to get green, light_green, dark_green
--   This lets us simplify the generation process by only having to define a single color in most cases
function ColorGroup(main, ...)
	return { main, ... }
end

local generated_textures = {}
local texture_ids = {}
local templates = {}

function TextureTemplate(id, file, colors)
	assert(templates[id] == nil, "Template '", id, "' already defined.")
	templates[id] = {
		id = id,
		file = file,
		colors = colors,
	}
end

function GenerateTexture(id, template_id, colors)
	assert(texture_ids[id] == nil, "Texture '", id, "' already defined.")
	texture_ids[id] = true
	local path = "./assets/textures/" .. string.lower(id) .. ".png"
	table.insert(generated_textures, {
		id = string.upper(id),
		template_id = template_id,
		colors = colors,
		path = path,
	})
end

function GetTexturesGroupByTemplates()
	local result = {}
	for _, value in ipairs(generated_textures) do
		if result[value.template_id] == nil then
			assert(templates[value.template_id] ~= nil, "Template '" .. value.template_id .. "' is undefined.")
			result[value.template_id] = {
				template = templates[value.template_id],
				textures = {},
			}
		end
		table.insert(result[value.template_id].textures, value)
	end
	return result
end
