#ifndef UI_WIDGETS_
#define UI_WIDGETS_

#include "linalg.h"
#include "texture_registry.h"
#include <raylib.h>
#include <stdbool.h>


typedef struct {
	Vec2i head;
	float padding;
	float field_gap;
} UIContext;

UIContext ui_context(Vec2i head);

// ////////////////////////////////////////
// //////////////// WINDOW ////////////////
// ////////////////////////////////////////

typedef enum {
	WINDOW_EVENT_NONE,
	WINDOW_EVENT_CLOSE,
} WindowEvent;

typedef struct {
	bool visible;
	char *title;
	Rect2i rectangle;
	bool dragged;
	Vec2i drag_offset;
} Window;

WindowEvent ui_window(Window *window);
UIContext window_context(Window *window);


// ////////////////////////////////////////
// //////////////// BUTTON ////////////////
// ////////////////////////////////////////

typedef enum {
	BUTTON_IDLE,
	BUTTON_HOVER,
	BUTTON_PRESSED,
	BUTTON_DISABLED,
} ButtonState;

typedef enum {
	BUTTON_PRIMARY,
	BUTTON_GREEN,
	BUTTON_RED,
	BUTTON_YELLOW,
} ButtonVariant;

typedef struct {
	ButtonVariant variant;
	bool disabled;
	int label_offset_x;
	int label_offset_y;
} ButtonOptions;

bool ui_button_ex(Rect2i rectangle, const char* label, ButtonOptions options);
#define ui_button(rectangle, label, ...) \
	ui_button_ex((rectangle), (label), (ButtonOptions){__VA_ARGS__})

// ////////////////////////////////////////
// //////////////// FIELD /////////////////
// ////////////////////////////////////////

typedef struct {
	const char *text;
	// NOTE: At some point, try to use our own Color struct, and not raylibs!
	Color color;
} UIField;
#define UIFIELD(text, color) ((UIField){text, color})

void ui_field(UIContext *context, const char *label, UIField value);

// ////////////////////////////////////////
// //////////////// SLOT //////////////////
// ////////////////////////////////////////

void ui_slot(Rect2i rectangle, bool selected);

// ////////////////////////////////////////
// //////////////// Textures //////////////
// ////////////////////////////////////////

void ui_texture(TextureId texture, Rect2i rectangle);

#endif // UI_WIDGETS_
