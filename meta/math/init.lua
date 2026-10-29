require("meta.math.utils")
require("meta.math.vector")
require("meta.math.basic")
require("meta.math.rect")

require("meta.math.ffi")

local header_guard = "LINALG_H"
local filename = "linalg"
local src_includes = { "<stdio.h>", "<assert.h>", "<stddef.h>" }
local header_includes = { "<stdbool.h>" }

local function generate()
	Basic()

	Vector({ name = "Vec2i", component = "int", dimension = 2 })
	Vector({ name = "Vec3i", component = "int", dimension = 3 })
	Vector({ name = "Vec2", component = "float", dimension = 2 })
	Vector({ name = "Vec3", component = "float", dimension = 3 })

	Rect2({ name = "Rect2i", component = "int" })
	Rect2({ name = "Rect2", component = "float" })
end

return function()
	-- Header File Gen
	local header_file = filename .. ".h"
	io.output("./include/" .. header_file)
	MathHeaderBegin(header_guard)
	MathInclude(header_includes)
	generate()
	MathHeaderEnd(header_guard)
	io.close()

	-- Source File Gen
	io.output("./core/" .. filename .. ".c")
	MathSourceBegin(header_file)
	MathInclude(src_includes)
	generate()
	MathSourceEnd()
	io.close()

	io.output("./lua/cdef/" .. filename .. ".lua")
	GenerateMathFFI()
	io.close()

end
