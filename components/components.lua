require("components.registry")

Include("linalg.h")
Include("physics.h")
Include("display.h")

Flag("PlayerTag")

Component({ "Rigidbody", {
	id = "ColliderId",
	velocity = Type.vec2,
	on_ground = Type.bool,
	gravity = Type.float,
} })

ExternComponent("SpriteId")

TypeAliasComponent("Position", Type.vec2)
