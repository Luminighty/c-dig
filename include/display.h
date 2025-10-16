#ifndef DISPLAY_H
#define DISPLAY_H

#include "assets_registry.h"
#include "linalg.h"
#include <stdbool.h>
#include <stdint.h>


#define ZINDEX_SKIP 0
#define SPRITE_MAX 32

typedef char ZIndex;
typedef uint32_t SpriteId;


typedef struct {
	SpriteId id;
	TextureId texture;
	Vec2 position;
	Vec2 offset;
	bool enabled;
	bool alive;
} Sprite;


typedef struct {
	Sprite sprites[SPRITE_MAX];
	SpriteId sprite_count;
	Vec2 camera;
} DisplayServer;


void display_init();
void display_destroy();
void display_render();

void camera_set_position(Vec2 position);
Vec2 camera_get_position();

SpriteId sprite_create(TextureId texture, Vec2 position, Vec2 offset);
void sprite_destroy(SpriteId sprite);

void sprite_set_texture(SpriteId sprite, TextureId texture);
void sprite_set_position(SpriteId sprite, Vec2 position);
void sprite_set_offset(SpriteId sprite, Vec2 offset);
void sprite_set_enabled(SpriteId sprite, bool enabled);


#endif // DISPLAY_H
