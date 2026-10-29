local ffi = require("ffi")


ffi.cdef[[
	Entity entity_create(World* world);
	void entity_destroy(World* world, Entity entity);
	bool entity_is_alive(World* world, Entity entity);
	bool entity_has_component(World *world, Entity entity, ComponentType component);
	bool entity_has_components(World *world, Entity entity, ComponentBitmap components);

	Entity query_begin(World* world, ComponentBitmap query);
	void query_next(World* world, Entity* entity, ComponentBitmap query);
	bool query_has_next(World* world, Entity* entity);
]]

local Entity = {}

function Entity.query(...)
	local bitmap = ffi.new("ComponentBitmap")
	for _, v in ipairs({...}) do
		ffi.C.componentbitmap_add(bitmap, v)
	end
	return bitmap
end

function Entity.foreach(world, query)
	local ent = ffi.new("Entity[1]")
	ent[0] = ffi.C.query_begin(world, query)
	local first = true
	return function ()
		if not first then
			ffi.C.query_next(world, ent, query)
		end
		first = false
		if not ffi.C.query_has_next(world, ent) then
			return nil
		end
		return ent[0]
	end
end

return Entity
