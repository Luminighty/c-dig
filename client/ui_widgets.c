#include "client/ui_widgets.h"
#include "assets.h"
#include "client/input.h"
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
	} else if (!options.skip_hitcheck) {
		bool hovering = CheckCollisionPointRec(
			to_rayvec(input.ui_mouse),
			to_rayrect(rectangle)
		);
		if (hovering) {
			state = IsMouseButtonDown(0) ? BUTTON_PRESSED : BUTTON_HOVER;
			if (IsMouseButtonReleased(0)) {
				result = true;
				state = BUTTON_PRESSED;
			}
		}
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
		.x = window->box.x + 8,
		.y = window->box.y + 22,
	});
}


WindowEvent ui_window(Window *window) {
	WindowEvent result = WINDOW_EVENT_NONE;
	static const NPatchInfo WINDOW_NPATCH = {
		.source = { 0, 0, 42, 42 },
		14, 14, 14, 14, NPATCH_NINE_PATCH
	};
	Vec2i ui_mousei = {.x = input.ui_mouse.x, .y = input.ui_mouse.y};
	if (window->dragged) {
		window->box.pos = vec2i_sub(ui_mousei, window->drag_offset);
		if (IsMouseButtonReleased(0))
			window->dragged = false;
	}
	Rectangle close_button_rect = {
		.x = window->box.x + window->box.w - 14,
		.y = window->box.y,
		.width = 14,
		.height = 14,
	};
	Rectangle title_rectangle = {
		.x = window->box.x,
		.y = window->box.y,
		.width = window->box.w,
		.height = 14,
	};


	enum {
		WINDOW_CLOSE_IDLE,
		WINDOW_CLOSE_HOVER,
		WINDOW_CLOSE_HOLD,
	} close_state = WINDOW_CLOSE_IDLE;
	Vector2 mouse = to_rayvec(input.ui_mouse);
	if (CheckCollisionPointRec(mouse, close_button_rect)) {
		close_state = WINDOW_CLOSE_HOVER;
		if (IsMouseButtonDown(0))
			close_state = WINDOW_CLOSE_HOLD;
		if (IsMouseButtonReleased(0))
			result = WINDOW_EVENT_CLOSE;
	} else if (CheckCollisionPointRec(mouse, title_rectangle)) {
		if (IsMouseButtonPressed(0)) {
			window->drag_offset = vec2i_sub(ui_mousei, window->box.pos);
			window->dragged = true;
		}
	}

	input.ui_hovering = CheckCollisionPointRec(mouse, to_rayrect(window->box));
	DrawTextureNPatch(
		assets_texture(TEXTURE_UI_WINDOW), WINDOW_NPATCH,
		to_rayrect(window->box), (Vector2){0, 0}, 0, WHITE
	);
	static const Rectangle CLOSE_RECT_IDLE = {
		.x = 42, .y = 0, .width = 14, .height = 14,
	};
	static const Rectangle CLOSE_RECT_HOVER = {
		.x = 42, .y = 14, .width = 14, .height = 14,
	};
	static const Rectangle CLOSE_RECT_HOLD = {
		.x = 42, .y = 28, .width = 14, .height = 14,
	};
	DrawTextureRec(
		assets_texture(TEXTURE_UI_WINDOW),
		close_state == WINDOW_CLOSE_IDLE ? CLOSE_RECT_IDLE :
		close_state == WINDOW_CLOSE_HOVER ? CLOSE_RECT_HOVER :
			CLOSE_RECT_HOLD,
		(Vector2){ close_button_rect.x, close_button_rect.y },
		WHITE
	);
	Vector2 title_position = (Vector2){
		.x = window->box.x + 4,
		.y = window->box.y + 1,
	};
	DrawTextEx(GetFontDefault(), window->title, title_position, FONTSIZE, 1, RAYWHITE);
	return result;
}


static const Color UI_COLORS[] = {
	[UI_COLOR_BASE] = RAYWHITE,
	[UI_COLOR_YELLOW] = YELLOW,
	[UI_COLOR_GREEN] = GREEN,
	[UI_COLOR_RED] = RED,
	[UI_COLOR_BLUE] = BLUE,
	[UI_COLOR_GRAY] = GRAY,
};

void ui_field(UIFieldProps props, const char *label, const char *value) {
	DrawText(label, props.position.x, props.position.y, FONTSIZE, RAYWHITE);
	DrawText(
		value,
		props.position.x + props.gap,
		props.position.y,
		FONTSIZE, UI_COLORS[props.color]
	);
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

