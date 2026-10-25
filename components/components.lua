require("components.registry")

Include("linalg.h")
Include("physics.h")
Include("display.h")
Include("netentity_id.h")

Flag("PlayerTag")

Component({ "RandomWalk", {
	{ "target" , Type.vec2 },
	{ "target_entity" , Type.Entity },
} })

-- Eventually we need to replace this with a better player index
Component({ "Controllable", {
	{ "owner" , Type.int },
} })

Component({ "NetworkEntity", {
	{ "id" , "NetEntityId" },
	{ "owner" , "uint32_t" },
	{ "dirty" , Type.bool },
} })

Component({ "Rigidbody", {
	{ "id" , "ColliderId" },
	{ "velocity" , Type.vec2 },
	{ "gravity" , Type.float },
	{ "on_ground" , Type.bool },
	{ "static_body" , Type.bool },
} })

Component({ "Light", {
	{ "intensity" , Type.int },
} })

ExternComponent("SpriteId")

TypeAliasComponent("Position", Type.vec2)

require("components.items")
