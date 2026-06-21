#include "ui_widgets.h"
#include "assets.h"
#include "config.h"
#include "input.h"
#include "linalg.h"
#include "texture_registry.h"
#include <raylib.h>
#include <stdbool.h>

static const int FONTSIZE = 10;

static inline Vector2 to_rayvec(Vec2 vec) { return (Vector2){.x = vec.x, .y= vec.y }; }
static inline Rectangle to_rayrect(Rect2i rect) { return (Rectangle){ .x = rect.x, .y = rect.y, .width = rect.w, .height = rect.h }; }

UIContext ui_context(Vec2i head) {
	return (UIContext){
		.head = head,
		.padding = 14,
		.field_gap = 48,
	};
}


static const int BUTTON_TEXTURE_SIZE = 12;


bool ui_button_ex(Rect2i rectangle, const char* label, ButtonOptions options) {
	bool result = false;

	ButtonState state = BUTTON_IDLE;
	if (options.disabled) {
		state = BUTTON_DISABLED;
	} else if (CheckCollisionPointRec(to_rayvec(input.ui_mouse), to_rayrect(rectangle))) {
		input.ui_hovering = true;
		state = IsMouseButtonDown(0) ? BUTTON_PRESSED : BUTTON_HOVER;
		result = IsMouseButtonReleased(0);
	}

	NPatchInfo npatch = {
		.source = {
			.x = BUTTON_TEXTURE_SIZE * options.variant,
			.y = BUTTON_TEXTURE_SIZE * state,
			.width = BUTTON_TEXTURE_SIZE,
			.height = BUTTON_TEXTURE_SIZE,
		},
		4, 4, 4, 4, NPATCH_NINE_PATCH
	};
	if (options.label_offset_x == 0) options.label_offset_x = 4;
	if (options.label_offset_y == 0) options.label_offset_y = 4;

	Vector2 label_pos = (Vector2){
		.x = rectangle.x + options.label_offset_x,
		.y = rectangle.y + options.label_offset_y,
	};

	DrawTextureNPatch(assets_texture(TEXTURE_UI_BUTTON), npatch, to_rayrect(rectangle), (Vector2){0, 0}, 0, WHITE);
	DrawTextEx(GetFontDefault(), label, label_pos, FONTSIZE, 1, RAYWHITE);

	return result;
}


UIContext window_context(Window *window) {
	return ui_context((Vec2i){
		.x = window->rectangle.x + 8,
		.y = window->rectangle.y + 22,
	});
}


WindowEvent ui_window(Window *window) {
	static const NPatchInfo WINDOW_NPATCH = {
		.source = { 0, 0, 42, 42 },
		14, 14, 14, 14, NPATCH_NINE_PATCH
	};
	if (!window->visible) return WINDOW_EVENT_NONE;

	Vec2i ui_mousei = {input.ui_mouse.x, input.ui_mouse.y};
	if (window->dragged) {
		window->rectangle.pos = vec2i_sub(ui_mousei, window->drag_offset);
		if (IsMouseButtonReleased(0))
			window->dragged = false;
	}
	Rectangle close_button_rect = {
		.x = window->rectangle.x + window->rectangle.w - 14,
		.y = window->rectangle.y,
		.width = 14,
		.height = 14,
	};
	Rectangle title_rectangle = {
		.x = window->rectangle.x,
		.y = window->rectangle.y,
		.width = window->rectangle.w,
		.height = 14,
	};
	Vector2 mouse = to_rayvec(input.ui_mouse);
	if (CheckCollisionPointRec(mouse, close_button_rect)) {
		if (IsMouseButtonReleased(0))
			window->visible = false;
	} else if (CheckCollisionPointRec(mouse, title_rectangle)) {
		if (IsMouseButtonPressed(0)) {
			window->drag_offset = vec2i_sub(ui_mousei, window->rectangle.pos);
			window->dragged = true;
		}
	}

	input.ui_hovering = CheckCollisionPointRec(mouse, to_rayrect(window->rectangle));

	DrawTextureNPatch(
		assets_texture(TEXTURE_UI_WINDOW), WINDOW_NPATCH,
		to_rayrect(window->rectangle), (Vector2){0, 0}, 0, WHITE
	);
	Vector2 title_position = (Vector2){
		.x = window->rectangle.x + 4,
		.y = window->rectangle.y + 2,
	};
	DrawTextEx(GetFontDefault(), window->title, title_position, FONTSIZE, 1, RAYWHITE);
	return WINDOW_EVENT_NONE;
}


void ui_field(UIContext *context, const char *label, UIField value) {
	DrawText(label, context->head.x, context->head.y, FONTSIZE, RAYWHITE);
	DrawText(
		value.text,
		context->head.x + context->field_gap, 
		context->head.y, 
		FONTSIZE, value.color
	);
	context->head.y += context->padding;
}


void ui_slot(Rect2i rectangle, bool selected) {
	static const Rectangle BASE = { 0, 0, 24, 24 };
	static const Rectangle SELECTED = { 24, 24, 24, 24 };
	NPatchInfo slot_patch = {
		.source = selected ? SELECTED : BASE,
		8, 8, 8, 8, NPATCH_NINE_PATCH
	};
	DrawTextureNPatch(
		assets_texture(TEXTURE_UI_HOTBAR), slot_patch,
		to_rayrect(rectangle), (Vector2){0, 0}, 0, WHITE
	);
}


void ui_texture(TextureId texture, Rect2i rectangle) {
	Texture2D t = assets_texture(texture);
	DrawTexturePro(
		t, (Rectangle){0, 0, t.width, t.height},
		to_rayrect(rectangle), (Vector2){0, 0}, 0, WHITE
	);
}
