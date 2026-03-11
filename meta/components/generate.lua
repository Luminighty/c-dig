require("components.registry")
require("components.components")

require("meta.components.component_header")
require("meta.components.component_source")

return function()
	local components = GetComponents()
	local includes = GetIncludes()

	GenerateHeaders(components, includes, "./include/components.h")
	GenerateSource(components, "./src/components.c")
end
