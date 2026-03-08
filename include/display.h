#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include "linalg.h"
#include "assets_registry.h"

#include <stdbool.h>
#include <stdint.h>


#define SPRITE_MAX 512

typedef uint32_t ZIndex;
typedef uint32_t SpriteId;



typedef enum {
	SPRITE_FLAG_ENABLED = 1 << 0,
	SPRITE_FLAG_ALIVE   = 1 << 1,
	SPRITE_FLAG_FLIP_X  = 1 << 2,
	SPRITE_FLAG_FLIP_Y  = 1 << 3,
} SpriteFlag;


typedef struct {
	SpriteId parent;
	TextureId texture;
	Vec2 position;
	Vec2 offset;
	SpriteFlag flags;
	RenderLayer layer;
	ZIndex z_index;
} Sprite;


typedef struct {
	Sprite sprites[SPRITE_MAX];
	SpriteId sorted[SPRITE_MAX];
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
void sprite_set_parent(SpriteId sprite, SpriteId new_parent);

void sprite_set_zindex(SpriteId sprite, ZIndex z_index);
void sprite_set_layer(SpriteId sprite, RenderLayer layer);
void sprite_set_flip_x(SpriteId sprite, bool flip_x);
void sprite_set_flip_y(SpriteId sprite, bool flip_y);


#endif // DISPLAY_H
