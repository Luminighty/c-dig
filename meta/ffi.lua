require("meta.utils")

local _, MODULE_PATH = ...

local function HeaderIntoFFI(header, outfile)
	io.output(outfile)
	FileMetaComment(MODULE_PATH, "--")
	WriteLuaRequire("ffi")
	io.write("\n")
	io.write("ffi.cdef[[\n")
		ReadHeaderIntoFFI(header)
	io.write("]]\n\n")
	io.close()
end


return function ()
	HeaderIntoFFI("./include/client/ui_widgets.h", "./lua/cdef/ui_widgets.lua")
	HeaderIntoFFI("./include/client/input.h", "./lua/cdef/input.lua")
end
