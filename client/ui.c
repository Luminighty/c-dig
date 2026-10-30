#include "client/ui.h"
#include "client/client_app.h"
#include "game.h"
#include "linalg.h"
#include "mineral.h"
#include "client/player.h"
#include "texture_registry.h"
#include "client/ui_widgets.h"
#include "utils.h"
#include <raylib.h>
#include <stdbool.h>


static TextureId slot_texture[] = {
	[PLAYER_TOOL_PICKAXE] = TEXTURE_ITEMS_PICKAXE,
	[PLAYER_TOOL_WOOD] = TEXTURE_ITEMS_WOOD,
	[PLAYER_TOOL_STONE] = TEXTURE_ITEMS_STONE,
	[PLAYER_TOOL_TORCH] = TEXTURE_ITEMS_TORCH,
	[PLAYER_TOOL_CHEST] = TEXTURE_FURNITURE_CHEST,
	[PLAYER_TOOL_FURNACE] = TEXTURE_FURNITURE_FURNACE,
	[PLAYER_TOOL_WORKBENCH] = TEXTURE_FURNITURE_WORKBENCH,
	[PLAYER_TOOL_ANVIL] = TEXTURE_FURNITURE_ANVIL,
	[PLAYER_TOOL_LADDER] = TEXTURE_FURNITURE_LADDER,
};


static inline void render_slots() {
	for (int i = 0; i < PLAYER_TOOLBAR_SIZE; i++) {
		PlayerToolSlot *tool = &client_app.player.tools[i];
		bool is_selected = client_app.player.current_tool == i;
		Rect2i rect = (Rect2i){.x = 10 + i * 37, .y = 10, .w = 32, .h = 32};
		ui_slot(rect, is_selected);
		ui_texture(slot_texture[tool->kind], rect);
	}
}



typedef struct {
	bool enabled;
	int power;
} DummySmelter;

// static Window smelter_window = {
// 	.visible = true,
// 	.title = "Dummy-D",
// 	.rectangle = {.x = 100, .y = 100, .w = 120, .h = 100}
// };
// static Window bag_window = {
// 	.visible = true,
// 	.title = "Bag",
// 	.rectangle = {.x = 250, .y = 100, .w = 240, .h = 50}
// };
// static DummySmelter smelter = { .enabled = false, .power = 100};
// static void dummy_windows();


void ui_render(UI *ui) {
	// UNUSED(ui);
	// render_slots();
	// dummy_windows();
	// if (smelter.enabled && game.tick % 30 == 0)
	// 	smelter.power--;
}


// static UIField smelter_power(DummySmelter *smelter) {
// 	Color color = BLUE;
// 	if (smelter->power < 100)
// 		color = GREEN;
// 	if (smelter->power < 50)
// 		color = YELLOW;
// 	if (smelter->power < 20)
// 		color = RED;
// 	return UIFIELD(
// 		TextFormat("%d %%", smelter->power),
// 		color
// 	);
// }


// static UIField smelter_ore(DummySmelter *smelter) {
// 	Color color = RAYWHITE;
// 	if (smelter->ore == ORE_NONE)
// 		color = GRAY;
// 	return UIFIELD(ore_label[smelter->ore], color);
// }


// static UIField smelter_status(DummySmelter *smelter) {
// 	if (smelter->power == 0)
// 		return UIFIELD("No Power", GRAY);
// 	if (smelter->enabled)
// 		return UIFIELD("Online", GREEN);
// 	return UIFIELD("Offline", RED);
// }


// void smelter_ui(DummySmelter *smelter, Window *window) {
// 	if (!window->visible)
// 		return;
// 	ui_window(window);
//
// 	UIContext ctx = window_context(window);
//
// 	ui_field(&ctx, "Status", smelter_status(smelter));
// 	ui_field(&ctx, "Power", smelter_power(smelter));
// 	// ui_field(&ctx, "Ore", smelter_ore(smelter));
//
// 	ctx.head.y += 8;
//
// 	bool start_pressed = ui_button(
// 		((Rect2i){ .x = ctx.head.x, .y = ctx.head.y, .w = 48, .h = 18 }), "Start",
// 		.variant = BUTTON_GREEN,
// 		.disabled = smelter->enabled || smelter->power <= 0
// 	);
// 	bool stop_pressed = ui_button(
// 		((Rect2i){ .x = ctx.head.x + 52, .y = ctx.head.y, .w = 48, .h = 18 }), "Stop",
// 		.variant = BUTTON_RED,
// 		.disabled = !smelter->enabled
// 	);
// 	if (start_pressed) smelter->enabled = true;
// 	if (stop_pressed) smelter->enabled = false;
// }
//
// static inline Color to_raylib_color(uint32_t color) {
// 	return (Color) {
// 		.r = (color & 0xFF0000) >> 16,
// 		.g = (color & 0x00FF00) >> 8,
// 		.b = (color & 0x0000FF) >> 0,
// 		.a = 0xFF,
// 	};
// }

// void bag_ui(PlayerBag *bag, Window *window) {
// 	if (!window->visible)
// 		return;
// 	ui_window(window);
// 	UIContext context = window_context(window);
//
// 	int width = window->box.w - 16;
// 	DrawRectangle(
// 		context.head.x, context.head.y, 
// 		width, 18,
// 		GetColor(0x222222)
// 	);
// 	width -= 2;
// 	int x = context.head.x + 1;
// 	for (int i = 0; i < MINERAL__SIZE; i++) {
// 		uint16_t amount = bag->amount[i];
// 		float percentage = (float)amount / (float)bag->size;
// 		int w = width * percentage;
//
// 		DrawRectangle(
// 			x, context.head.y + 1,
// 			w, 16,
// 			to_raylib_color(mineral_color(i))
// 		);
// 		x += w;
// 	}
// }

static void dummy_windows() {
	// smelter_ui(&smelter, &smelter_window);
	// bag_ui(&client_app.player.bag, &bag_window);
}
//
//
// void smelter_toggle_ui() {
// 	smelter_window.visible = !smelter_window.visible;
// }
//
