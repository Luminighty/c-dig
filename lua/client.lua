package.path = package.path .. ";./lua/?/init.lua;./lua/?.lua"

local ffi = require("ffi")
local C = require("cdef")

ffi.cdef[[
	typedef struct player_controller PlayerController;
	Entity player_entity(PlayerController* player);
]]

local torch
local spoopy

local player_entity

function OnClientInit()
	local offset = -2
	local tile = ffi.new("Vec2i", { x = 64 * 2 + offset, y = 64 * 2 })
	torch = C.entity_create_torch(World, tile, 32)

	player_entity = C.player_entity(Player)
end

function OnClientUnload()
	C.entity_destroy(World, torch);
	C.entity_destroy(World, spoopy);
end

function OnPlayerEntityFound(entity)
	local spoopy_spawn = C.tile_to_world_coord({ x = 15, y = 64 * 4 - 40 });
	spoopy = C.entity_create_spoopy_fren(World, spoopy_spawn, entity)
	print("Spoopy: ", spoopy)
end


function OnClientUpdate()
	if not C.entity_is_alive(World, player_entity) then
		player_entity = C.player_entity(Player)
		if C.entity_is_alive(World, player_entity) then
			OnPlayerEntityFound(player_entity)
		end
	end
end

function OnClientUI()
end
