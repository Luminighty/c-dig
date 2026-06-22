require("components.registry")

Include("linalg.h")
Include("physics.h")
Include("display.h")

Flag("PlayerTag")

Component({ "RandomWalk", {
	target = Type.vec2,
} })

Component({ "Rigidbody", {
	id = "ColliderId",
	velocity = Type.vec2,
	on_ground = Type.bool,
	gravity = Type.float,
} })

Component({ "Light", {
	intensity = Type.int,
} })

ExternComponent("SpriteId")

TypeAliasComponent("Position", Type.vec2)

require("components.items")
