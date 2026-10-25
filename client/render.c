#include "client/render.h"

#include <raylib.h>

#include "assets.h"
#include "client/client_app.h"
#include "debug.h"
#include "display.h"
#include "linalg.h"
#include "config.h"
#include "game.h"

static Camera2D camera = {0};

#define GAME_TILE_WIDTH (GAME_WIDTH / TILE_PIXEL_SIZE)
#define GAME_TILE_HEIGHT (GAME_HEIGHT / TILE_PIXEL_SIZE)

static DisplayServer *display;
static inline void display_render();

void client_render() {
	Rectangle WINDOW = {
		.width = WINDOW_DIMENSION.x, .height = WINDOW_DIMENSION.y 
	};
	Rectangle GAME_SOURCE = { .width = GAME_WIDTH, .height = -GAME_HEIGHT };
	Rectangle UI_SOURCE = { .width = (int)(WINDOW.width / UI_SCALE), .height = -(int)(WINDOW.height / UI_SCALE) };
	Rect2 _GAME_TARGET = game_viewport();
	Rectangle GAME_TARGET = {
		.x = _GAME_TARGET.x, .y = _GAME_TARGET.y,
		.width = _GAME_TARGET.w, .height = _GAME_TARGET.h,
	};
	BeginTextureMode(client_app.ui_target);
	ClearBackground(BLANK);
	// ui_render(&game.ui);
	EndTextureMode();

	BeginTextureMode(client_app.render_target);
	ClearBackground(BLACK);
	display_render();
	EndTextureMode();
	DrawTexturePro(client_app.render_target.texture, GAME_SOURCE, GAME_TARGET, (Vector2){0}, 0, WHITE);

	DrawTexturePro(client_app.ui_target.texture, UI_SOURCE, WINDOW, (Vector2){0}, 0, WHITE);

	debug_end();
	DrawFPS(10, 10);
}


void client_render_init() {
	camera.zoom = 1.0f;
	display = display_get_server();

	// WARN: We may need to resize this during runtime, 
	// 	in case the window gets resized!
	Rectangle WINDOW = { .width = WINDOW_DIMENSION.x, .height = WINDOW_DIMENSION.y };
	int ui_width = WINDOW.width / UI_SCALE;
	int ui_height = WINDOW.height / UI_SCALE;
	client_app.render_target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
	client_app.ui_target = LoadRenderTexture(ui_width, ui_height);
}


void client_render_destroy() {

}

static inline Rect2i visible_tiles_rect() {
	Vec2i center = (Vec2i){
		.x = display->camera.x / TILE_PIXEL_SIZE,
		.y = display->camera.y / TILE_PIXEL_SIZE,
	};
	int from_x = center.x;
	int from_y = center.y;
	int to_x = from_x + GAME_TILE_WIDTH + 1;
	int to_y = from_y + GAME_TILE_HEIGHT + 1;
	return (Rect2i){
		.x = from_x - 1, .y = from_y - 1,
		.w = to_x - from_x, .h = to_y - from_y
	};
}



static void light_render() {
	Rect2i rect = visible_tiles_rect();
	RECT2I_FOREACH(rect, x, y) {
		uint8_t level = map_get_light_level(game.map, x, y);

		Color color = ColorAlpha(BLACK, (LIGHT_LEVEL_FULL - level) / (float)LIGHT_LEVEL_FULL);
		DrawRectangle(
			(x * TILE_PIXEL_SIZE),
			(y * TILE_PIXEL_SIZE),
			TILE_PIXEL_SIZE,
			TILE_PIXEL_SIZE,
			color
		);
	}
}


static void map_render(RenderLayer layer) {
	Rect2i rect = visible_tiles_rect();
	RECT2I_FOREACH(rect, x, y) {
		Tile tile = map_get(game.map, x, y);
		if (tile_render_layer(tile) != layer)
			continue;

		TextureId texture_id = tile_texture(tile);
		if (texture_id == TEXTURE__NONE)
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
	}
}


static inline Sprite* get_sprite(SpriteId sprite) {
	if (sprite >= display->sprite_count)
		return &display->sprites[0];
	return &display->sprites[sprite];
}


typedef struct {
	bool flip_x;
	Vec2 position;
} SpriteSettings;


static inline SpriteSettings get_global_settings(Sprite *sprite) {
	Sprite* current = sprite;
	SpriteSettings settings = {
		.position = current->position,
		.flip_x = (current->flags & SPRITE_FLAG_FLIP_X) != 0,
	};
	while (current->parent != 0) {
		current = get_sprite(current->parent);
		settings.position.x += current->position.x;
		settings.position.y += current->position.y;
		if (current->flags & SPRITE_FLAG_FLIP_X) {
			settings.flip_x = !settings.flip_x;
		}
	}
	return settings;
}


static inline void world_render();
static inline void display_render() {
	camera.target.x = display->camera.x;
	camera.target.y = display->camera.y;
	BeginMode2D(camera);
	world_render();

	player_render_interaction(&game, &client_app.player);
	light_render();
	
	physics_render();
	EndMode2D();
}


static inline void world_render() {
	RenderLayer layer = RENDER_LAYER_BACKGROUND;
	for (size_t i = 1; i < display->sprite_count; i++) {
		SpriteId id = display->sorted[i];
		Sprite* sprite = get_sprite(id);
		if (!(sprite->flags & (SPRITE_FLAG_ALIVE | SPRITE_FLAG_ENABLED)))
			continue;

		while (sprite->layer > layer) {
			map_render(layer++);
		}

		SpriteSettings settings = get_global_settings(sprite);
		Vec2 position = settings.position;
		Texture2D texture = assets_texture(sprite->texture);
		Rectangle dest = {
			.x = position.x + sprite->offset.x,
			.y = position.y + sprite->offset.y,
			texture.width,
			texture.height 
		};
		Rectangle source = {
			.x = 0,
			.y = 0,
			texture.width,
			texture.height,
		};
		if (settings.flip_x)
			source.width *= -1;
		DrawTexturePro(texture, source, dest, ((Vector2){0, 0}), 0.0f, WHITE);
	}
	while (layer < RENDER_LAYER_SIZE)
		map_render(layer++);
}

