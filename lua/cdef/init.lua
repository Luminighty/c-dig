local ffi = require("ffi")


ffi.cdef[[
	typedef union entity {
		uint32_t id;
		struct {
			uint16_t gen;
			uint16_t index;
		} as;
	} Entity;
	typedef uint32_t ColliderId;
	typedef uint32_t NetEntityId;
	typedef uint32_t ZIndex;
	typedef uint32_t SpriteId;
	typedef uint32_t SpriteSocketKey;
	typedef uint64_t TextureId;

	typedef struct world World;
	typedef struct map Map;
	typedef union entity Entity;
	typedef struct game {
		uint64_t tick;
		World *world;
		Map *map;
	} Game;
	extern Game game;
	// extern Game game;
]]

require("cdef.linalg")
require("cdef.Component")
require("cdef.prefabs")
require("cdef.Entity")
require("cdef.map")
require("cdef.ui_widgets")
require("cdef.input")

return ffi.C
