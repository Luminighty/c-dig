#include "display.h"
#include "texture_registry.h"
#include "config.h"
#include "linalg.h"
#include <assert.h>
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


static DisplayServer display = {0};
DisplayServer *display_get_server() {
	return &display;
}

static inline void fix_sorted_array(SpriteId id, size_t current_index);


void display_init() {
	display.sprite_count = 1;
}

void display_destroy() {
}


void camera_set_position(Vec2 position) { display.camera = position; }
Vec2 camera_get_position() { return display.camera; }


SpriteId sprite_create(TextureId texture, Vec2 position, Vec2 offset) {
	assert(display.sprite_count < SPRITE_MAX);
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
	printf("Sprite %d destroyed.\n", sprite);
	display.sprites[sprite].flags &= ~SPRITE_FLAG_ALIVE;
}


TextureId sprite_get_texture(SpriteId sprite) {
	return display.sprites[sprite].texture;
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

void sprite_socket_add(SpriteId sprite, SpriteSocketKey key, SpriteId socket_sprite) {
	sprite_set_parent(socket_sprite, sprite);
	assert(display.sprites[sprite].socket_count < SPRITE_SOCKET_MAX);
	Sprite *sprite_data = &display.sprites[sprite];
	// NOTE: Maybe we could just overwrite it if there is a duplicate
	for (size_t i = 0; i < sprite_data->socket_count; i++) {
		assert(sprite_data->sockets[i].key != key);
	}
	SpriteSocket *socket = &sprite_data->sockets[sprite_data->socket_count++];
	socket->key = key;
	socket->sprite = socket_sprite;
}

SpriteId sprite_socket_get(SpriteId sprite, SpriteSocketKey key) {
	Sprite *sprite_data = &display.sprites[sprite];
	for (size_t i = 0; i < sprite_data->socket_count; i++) {
		if (sprite_data->sockets[i].key == key)
			return sprite_data->sockets[i].sprite;
	}
	return 0;
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


void sprite_set_flip_x(SpriteId sprite, bool flip_x) {
	if (flip_x) {
		display.sprites[sprite].flags |= SPRITE_FLAG_FLIP_X;
	} else {
		display.sprites[sprite].flags &= ~SPRITE_FLAG_FLIP_X;
	}
}


void sprite_set_flip_y(SpriteId sprite, bool flip_y) {
	if (flip_y) {
		display.sprites[sprite].flags |= SPRITE_FLAG_FLIP_Y;
	} else {
		display.sprites[sprite].flags &= ~SPRITE_FLAG_FLIP_Y;
	}
}

