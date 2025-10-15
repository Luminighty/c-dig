#include "display.h"
#include "assets.h"
#include "assets_registry.h"
#include "config.h"
#include "game.h"
#include "linalg.h"
#include "map.h"
#include "tile.h"
#include <raylib.h>
#include <stdio.h>
#include "debug.h"
#include "collision.h"

static DisplayServer display = {0};
static Camera2D camera;


void display_init() {
	camera.zoom = 1.0f;
	display.sprite_count = 1;
}

void display_destroy() {
}


static void map_render() {
	static const int from_x = -SCREEN_WIDTH / 2;
	static const int from_y = -SCREEN_HEIGHT / 2;
	static const int to_x = SCREEN_WIDTH;
	static const int to_y = SCREEN_HEIGHT;

	Vec2i offset = (Vec2i){
		.x = display.camera.x / TILE_PIXEL_SIZE,
		.y = display.camera.y / TILE_PIXEL_SIZE,
	};
	Vec2i subtile_offset = (Vec2i){
		.x = (int)display.camera.x % TILE_PIXEL_SIZE,
		.y = (int)display.camera.y % TILE_PIXEL_SIZE,
	};
	for (int y = 0; y <= to_y; y++) {
	for (int x = 0; x <= to_x; x++) {
		Tile tile = map_get(game.map, offset.x + x + from_x, offset.y + y + from_y);
		TextureId texture_id = tile_textures[tile];
		if (texture_id == TEXTURE_NONE)
			continue;

		Color color = WHITE;
		if (tile == TILE_AIR)
			color = DARKGRAY;
		


		DrawTexture(
			assets_texture(texture_id),
			(x * TILE_PIXEL_SIZE) - subtile_offset.x,
			(y * TILE_PIXEL_SIZE) - subtile_offset.y,
			color
		);
	}}
}

void display_render() {
	camera.target.x = display.camera.x;
	camera.target.y = display.camera.y;
	map_render();
	BeginMode2D(camera);
	for (SpriteId id = 1; id < display.sprite_count; id++) {
		Sprite* sprite = &display.sprites[id];
		if (!sprite->alive || !sprite->enabled)
			continue;
		DrawTexture(
			assets_texture(sprite->texture),
			sprite->position.x + sprite->offset.x,
			sprite->position.y + sprite->offset.y,
			WHITE
		);
	}
	physics_render();
	EndMode2D();
}

inline void camera_set_position(Vec2 position) { display.camera = position; }

SpriteId sprite_create(TextureId texture, Vec2 position, Vec2 offset) {
	// TODO: Find an actual spot
	SpriteId id = display.sprite_count++;
	display.sprites[id] = (Sprite){
		.id = id,
		.texture = texture,
		.position = position,
		.offset = offset,
		.enabled = true,
		.alive = true
	};
	return id;
}

void sprite_destroy(SpriteId sprite) {
	display.sprites[sprite].alive = false;
}

inline void sprite_set_texture(SpriteId sprite, TextureId texture) {
	display.sprites[sprite].texture = texture;
}
inline void sprite_set_position(SpriteId sprite, Vec2 position) {
	display.sprites[sprite].position = position;
}
inline void sprite_set_offset(SpriteId sprite, Vec2 offset) {
	display.sprites[sprite].offset = offset;
}
inline void sprite_set_enabled(SpriteId sprite, bool enabled) {
	display.sprites[sprite].enabled = enabled;
}
