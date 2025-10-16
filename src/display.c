#include "display.h"
#include "assets.h"
#include "assets_registry.h"
#include "config.h"
#include "game.h"
#include "linalg.h"
#include "map.h"
#include "tile.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include "debug.h"
#include "collision.h"

static DisplayServer display = {0};
static Camera2D camera = {0};


void display_init() {
	camera.zoom = 1.0f;
	display.sprite_count = 1;
}

void display_destroy() {
}


static void map_render() {
	Vec2i center = (Vec2i){
		.x = display.camera.x / TILE_PIXEL_SIZE,
		.y = display.camera.y / TILE_PIXEL_SIZE,
	};
	int from_x = center.x;
	int from_y = center.y;
	int to_x = from_x + SCREEN_WIDTH + 1;
	int to_y = from_y + SCREEN_HEIGHT + 1;

	for (int y = from_y; y <= to_y; y++) {
	for (int x = from_x; x <= to_x; x++) {
		Tile tile = map_get(game.map, x, y);
		TextureId texture_id = tile_textures[tile];
		if (texture_id == TEXTURE_NONE)
			continue;

		Color color = WHITE;
		if (tile == TILE_AIR)
			color = DARKGRAY;
		DrawTexture(
			assets_texture(texture_id),
			(x * TILE_PIXEL_SIZE),
			(y * TILE_PIXEL_SIZE),
			color
		);
	}}
}

void display_render() {
	camera.target.x = display.camera.x;
	camera.target.y = display.camera.y;
	BeginMode2D(camera);
	map_render();
	for (SpriteId id = 1; id < display.sprite_count; id++) {
		Sprite* sprite = &display.sprites[id];
		if (!sprite->alive || !sprite->enabled)
			continue;
		DrawTexture(
			assets_texture(sprite->texture),
			round(sprite->position.x + sprite->offset.x),
			round(sprite->position.y + sprite->offset.y),
			WHITE
		);
	}
	physics_render();
	EndMode2D();
}

inline void camera_set_position(Vec2 position) { display.camera = position; }
inline Vec2 camera_get_position() { return display.camera; }

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
