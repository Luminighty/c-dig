local cc = nil
local libs = {}
local cflags = {}
local is_windows = package.config:sub(1,1) == "\\"
local slash = is_windows and "\\" or "/"
local is_stderr_tty = os.execute("test -t 2")


function CC(compiler) cc = compiler end

function Include(folder)
	table.insert(cflags, "-I" .. folder)
end

function CFLAG(...)
	local args = { ... }
	for _,v in ipairs(args) do
		table.insert(cflags, v)
	end
end

function Library(...)
	local args = { ... }
	for _,v in ipairs(args) do
		if type(v) == "table" then
			for _,l in ipairs(v) do
				table.insert(libs, "-l" .. l)
			end
		else
			table.insert(libs, "-l" .. v)
		end
	end
end

function Compile(file, object_file)
	return {
		file = object_file,
		process = ExecAsync(
			cc,
			"-c",
			"-o " .. object_file,
			file,
			table.concat(cflags, " "),
			table.concat(libs, " "),
			is_stderr_tty and " -fdiagnostics-color=always" or "",
			" 2>&1"
		)
	}
end

function CompileFolder(src_folder, object_folder)
	local objects = {}
	MakeDir(object_folder)
	for src in ListFiles(src_folder) do
		if src:match("%.c$") then
			local obj_file = src:gsub("%.c$", ".o")
			local path = Path(src_folder, src)
			local obj = Path(object_folder, obj_file)
			table.insert(objects, Compile(path, obj))
		end
	end
	return objects
end

function AwaitObjects(objects)
	local all_success = true
	local errors = {}
	local outputs = {}
	for _, object in ipairs(objects) do
		if object.process ~= nil then
			object.process:setvbuf("no")
			object.process:flush()
			local output = object.process:read("*all")
			local success, _, exit_code = object.process:close()
			if success and exit_code == 0 then
				table.insert(outputs, output)
			else
				all_success = false
				table.insert(errors, output)
			end
			object.process = nil
			object.success = success
		elseif not object.success then
			all_success = false
		end
	end
	if not all_success then
		return false, errors
	end
	return true, outputs
end


function Link(exec, ...)
	local object_lists = { ... }

	local ofiles = {}
	local objects = {}
	for _, list in ipairs(object_lists) do
		for _, object in ipairs(list) do
			table.insert(ofiles, object.file)
			table.insert(objects, object)
		end
	end
	local success, logs = AwaitObjects(objects)
	for _, log in ipairs(logs) do
		if #log > 0 then
			io.stderr:write(log)
			io.stderr:write("\n")
		end
	end
	if not success then
		os.exit(1)
	end

	Exec(
		cc,
		"-o " .. Path(exec),
		table.concat(ofiles, " "),
		table.concat(cflags, " "),
		table.concat(libs, " ")
	)
end

function Path(...)
	local nodes = {...}
	if is_windows then
		for i, node in ipairs(nodes) do
			nodes[i] = node:gsub("/", "\\")
		end
	end
	return table.concat(nodes, slash)
end

function ListFiles(folder)
	folder = Path(folder)
	local cmd = nil
	if is_windows then
		cmd = 'dir "' .. folder .. '" /b'
	else
		cmd = 'ls -1 "' .. folder .. '"'
	end
	local handle = assert(io.popen(cmd))
	return function()
		local line = handle:read("*l")
		if not line then
			handle:close()
			return nil
		end
		return line
	end
end

function MakeDir(folder)
	folder = Path(folder)
	if is_windows then
		Exec('mkdir "' .. folder .. '"')
	else
		Exec('mkdir -p "' .. folder .. '"')
	end
end

function CopyRecursive(src, dest)
	src = Path(src)
	dest = Path(dest)
	if is_windows then
		Exec('xcopy "' .. src .. '" "' .. dest .. '" /E /I /Y')
	else
		Exec('cp -r "' .. src .. '" "' .. dest .. '"')
	end
end

function ExecAsync(...)
	local cmd = table.concat({ ... }, " ")
	return assert(io.popen(cmd, "r"))
end

function Exec(...) ExecAsync(...):close() end

