local components = {}
local includes = {}

function GetComponents()
	return components
end

function GetIncludes()
	return includes
end

function Flag(name)
	table.insert(components, {
		class_name = name,
		fields = {},
		storage = Storage.Flag,
	})
end

function Component(config)
	table.insert(components, {
		class_name = config[1],
		fields = config[2],
		storage = Storage.Dense,
	})
end

function ExternComponent(name)
	table.insert(components, {
		class_name = name,
		extern = true,
		storage = Storage.Dense,
	})
end

function TypeAliasComponent(component_name, type)
	table.insert(components, {
		class_name = component_name,
		actual_type = type,
		storage = Storage.Dense,
	})
end

function Include(path)
	table.insert(includes, path)
end

Storage = {
	Dense = "DENSE",
	Flag = "FLAG",
}

Type = {
	int = "int",
	vec2 = "Vec2",
	bool = "bool",
	float = "float",
}
