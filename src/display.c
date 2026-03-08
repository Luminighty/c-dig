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
#include <stddef.h>
#include <stdio.h>
#include "debug.h"
// #include "collision.h"

static DisplayServer display = {0};
static Camera2D camera = {0};

static inline void fix_sorted_array(SpriteId id, size_t current_index);

void display_init() {
	camera.zoom = 1.0f;
	display.sprite_count = 1;
}

void display_destroy() {
}


static void map_render(RenderLayer layer) {
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
		if (tile_render_layer(tile) != layer)
			continue;

		TextureId texture_id = tile_texture(tile);
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

static inline Sprite* get_sprite(SpriteId sprite) {
	if (sprite >= display.sprite_count)
		return &display.sprites[0];
	return &display.sprites[sprite];
}

// TODO: Probably need to keep track of flipping, ect.
static inline Vec2 get_global_position(Sprite *sprite) {
	Sprite* current = sprite;
	Vec2 result = current->position;
	while (current->parent != 0) {
		current = get_sprite(sprite->parent);
		result.x += current->position.x;
		result.y += current->position.y;
	}
	return result;
}

void display_render() {
	camera.target.x = display.camera.x;
	camera.target.y = display.camera.y;
	BeginMode2D(camera);
	RenderLayer layer = RENDER_LAYER_BACKGROUND;
	map_render(layer);
	for (size_t i = 1; i < display.sprite_count; i++) {
		SpriteId id = display.sorted[i];
		Sprite* sprite = get_sprite(id);
		if (!(sprite->flags & (SPRITE_FLAG_ALIVE | SPRITE_FLAG_ENABLED)))
			continue;

		if (sprite->layer > layer) {
			layer = sprite->layer;
			map_render(layer);
		}

		Vec2 position = get_global_position(sprite);
		DrawTexture(
			assets_texture(sprite->texture),
			round(position.x + sprite->offset.x),
			round(position.y + sprite->offset.y),
			WHITE
		);
	}
	while (layer + 1 < RENDER_LAYER_SIZE)
		map_render(++layer);
	physics_render();
	EndMode2D();
}

void camera_set_position(Vec2 position) { display.camera = position; }
Vec2 camera_get_position() { return display.camera; }

SpriteId sprite_create(TextureId texture, Vec2 position, Vec2 offset) {
	// TODO: Find an actual spot
	SpriteId id = display.sprite_count++;
	display.sprites[id] = (Sprite){
		.parent = 0,
		.texture = texture,
		.position = position,
		.offset = offset,
		.layer = RENDER_LAYER_NORMAL,
		.flags = SPRITE_FLAG_ENABLED | SPRITE_FLAG_ALIVE,
	};
	display.sorted[id] = id;
	fix_sorted_array(id, id);
	
	return id;
}

void sprite_destroy(SpriteId sprite) {
	display.sprites[sprite].flags &= ~SPRITE_FLAG_ALIVE;
}

void sprite_set_texture(SpriteId sprite, TextureId texture) {
	display.sprites[sprite].texture = texture;
}
void sprite_set_position(SpriteId sprite, Vec2 position) {
	display.sprites[sprite].position = position;
}
void sprite_set_offset(SpriteId sprite, Vec2 offset) {
	display.sprites[sprite].offset = offset;
}
void sprite_set_enabled(SpriteId sprite, bool enabled) {
	if (enabled) {
		display.sprites[sprite].flags |= SPRITE_FLAG_ENABLED;
	} else {
		display.sprites[sprite].flags &= ~SPRITE_FLAG_ENABLED;
	}
}

void sprite_set_parent(SpriteId sprite, SpriteId new_parent) {
	display.sprites[sprite].parent = new_parent;
}

static inline bool is_render_left_below(SpriteId left_id, SpriteId right_id) {
	if (left_id == right_id) return false;
	if (left_id == 0) return true;
	if (right_id == 0) return false;

	Sprite* left = &display.sprites[left_id];
	Sprite* right = &display.sprites[right_id];

	if (left->layer < right->layer)
		return true;
	if (left->layer > right->layer)
		return false;
	if (left->z_index < right->z_index)
		return true;
	if (left->z_index > right->z_index)
		return false;
	return left_id < right_id;
}

static inline int find_sorted_index(SpriteId id) {
	size_t min = 1;
	size_t max = display.sprite_count - 1;

	while (min <= max) {
		size_t i = (min + max) / 2;
		if (display.sorted[i] == id)
			return i;
		if (is_render_left_below(display.sorted[i], id)) {
			min = i + 1;
		} else {
			max = i - 1;
		}
	}
	return 0;
}

static inline void fix_sorted_array(SpriteId id, size_t current_index) {
	if (current_index > 1 && is_render_left_below(id, display.sorted[current_index - 1])) {
		while (current_index > 1 && is_render_left_below(id, display.sorted[current_index - 1])) {
			display.sorted[current_index] = display.sorted[current_index - 1];
			current_index--;
		}
	} else if (current_index + 1< display.sprite_count && is_render_left_below(display.sorted[current_index + 1], id)) {
		while (current_index + 1 < display.sprite_count && is_render_left_below(display.sorted[current_index + 1], id)) {
			display.sorted[current_index] = display.sorted[current_index + 1];
			current_index++;
		}
	}
	display.sorted[current_index] = id;
}


void sprite_set_zindex(SpriteId sprite, ZIndex z_index) {
	// NOTE: Have to find the index before modifying the sorting values
	size_t index = find_sorted_index(sprite);
	display.sprites[sprite].z_index = z_index;
	fix_sorted_array(sprite, index);
}

void sprite_set_layer(SpriteId sprite, RenderLayer layer) {
	// NOTE: Have to find the index before modifying the sorting values
	size_t index = find_sorted_index(sprite);
	display.sprites[sprite].layer = layer;
	fix_sorted_array(sprite, index);
}
