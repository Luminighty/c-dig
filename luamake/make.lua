require("luamake.lib")

local OS = arg[1]
local TARGET = arg[2]

print("Target: ", TARGET)
print("    OS: ", OS)

CC("gcc")
Include("./include")
Library("m", "dl", "pthread") -- math, dynamic linking, threads
Library("raylib")

Include("./third_party/lua")
Library("luajit-5.1")
CFLAG("-Wl,-E") -- linux specific for lua ffi

local os_libs = {
	Windows = {"gdi32", "winmm"},
	Linux = {"GL", "X11"},
}
assert(os_libs[OS] ~= nil, "Unsupported OS \"" .. OS .. "\"")
Library(os_libs[OS])


CFLAG("-Wall", "-Wextra")
if TARGET == "release" then
	CFLAG("-O3", "-DNDEBUG", "-flto")
	CFLAG("-Werror")
else
	CFLAG("-g", "-O0")
	CFLAG("-DDEV_MODE")
	CFLAG("-fsanitize=address")
end

MakeDir("bin")
CopyRecursive("assets", "bin/")
CopyRecursive("lua", "bin/")

Exec("lua ./meta/generate.lua")

local core = CompileFolder("core", "build/core")
local client = CompileFolder("client", "build/client")
local server = CompileFolder("server", "build/server")

print("[Linking Client]")
Link("./bin/main", core, client)

print("[Linking Server]")
Link("./bin/server", core, server)


