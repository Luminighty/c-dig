require("components.registry")
require("components.components")

require("meta.components.header")
require("meta.components.source")
require("meta.components.ffi_cdef")


return function()
	local components = GetComponents()
	local includes = GetIncludes()

	GenerateHeaders(components, includes, "./include/components.h")
	GenerateSource(components, "./core/components.c")
	GenerateComponentFFI(components, "./lua/cdef/Component.lua")
	GeneratePrefabFFI("./lua/cdef/prefabs.lua")
end

