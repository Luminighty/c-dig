require("meta.utils")

local _, MODULE_PATH = ...

function GenerateMathFFI()
	FileMetaComment(MODULE_PATH, "--")

	WriteLuaRequire("ffi")
	io.write("\n")

	io.write("ffi.cdef[[\n")
		ReadHeaderIntoFFI("./include/linalg.h")
	io.write("]]\n\n")
end

