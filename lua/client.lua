package.path = package.path .. ";./lua/?/init.lua;./lua/?.lua"

local ffi = require("ffi")
local C = require("cdef")

local UI = require("ui")
require("ui.registry")

ffi.cdef[[
	typedef struct player_controller PlayerController;
	Entity player_entity(PlayerController* player);
]]

local torch
local spoopy

local player_entity

local smelter = {
	power = 100,
	enabled = false,
}
function OnClientInit()
	local offset = -2
	local tile = ffi.new("Vec2i", { x = 64 * 2 + offset, y = 64 * 2 })
	torch = C.entity_create_torch(World, tile, 32)

	player_entity = C.player_entity(Player)

	UI.Open("smelter", "custom-id", { smelter = smelter })
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
	if smelter.enabled and C.game.tick % 5 == 0 then
		smelter.power = smelter.power - 1
		if smelter.power <= 0 then smelter.enabled = false end
	end
end

function OnClientUI()
	UI.Draw(C.input.mouse)
end
