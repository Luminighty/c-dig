#include "ui.h"
#include "game.h"
#include "linalg.h"
#include "player.h"
#include "texture_registry.h"
#include "ui_widgets.h"
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
		PlayerToolSlot *tool = &game.player.tools[i];
		bool is_selected = game.player.current_tool == i;
		Rect2i rect = (Rect2i){10 + i * 37, 10, 32, 32};
		ui_slot(rect, is_selected);
		ui_texture(slot_texture[tool->kind], rect);
	}
}



typedef struct {
	bool enabled;
	int power;
} DummySmelter;

static Window smelter_window = {
	.visible = true,
	.title = "Dummy-D",
	.rectangle = {100, 100, 120, 100}
};
static DummySmelter smelter = { .enabled = false, .power = 100};
static void dummy_windows();


void ui_render(UI *ui) {
	render_slots();
	dummy_windows();
	if (smelter.enabled && game.tick % 30 == 0)
		smelter.power--;
}


static UIField smelter_power(DummySmelter *smelter) {
	Color color = BLUE;
	if (smelter->power < 100)
		color = GREEN;
	if (smelter->power < 50)
		color = YELLOW;
	if (smelter->power < 20)
		color = RED;
	return UIFIELD(
		TextFormat("%d %%", smelter->power),
		color
	);
}


// static UIField smelter_ore(DummySmelter *smelter) {
// 	Color color = RAYWHITE;
// 	if (smelter->ore == ORE_NONE)
// 		color = GRAY;
// 	return UIFIELD(ore_label[smelter->ore], color);
// }


static UIField smelter_status(DummySmelter *smelter) {
	if (smelter->power == 0)
		return UIFIELD("No Power", GRAY);
	if (smelter->enabled)
		return UIFIELD("Online", GREEN);
	return UIFIELD("Offline", RED);
}


void smelter_ui(DummySmelter *smelter, Window *window) {
	if (!window->visible)
		return;
	ui_window(window);

	UIContext ctx = window_context(window);

	ui_field(&ctx, "Status", smelter_status(smelter));
	ui_field(&ctx, "Power", smelter_power(smelter));
	// ui_field(&ctx, "Ore", smelter_ore(smelter));

	ctx.head.y += 8;

	bool start_pressed = ui_button(
		((Rect2i){ ctx.head.x, ctx.head.y, 48, 18 }), "Start",
		.variant = BUTTON_GREEN,
		.disabled = smelter->enabled || smelter->power <= 0
	);
	bool stop_pressed = ui_button(
		((Rect2i){ ctx.head.x + 52, ctx.head.y, 48, 18 }), "Stop",
		.variant = BUTTON_RED,
		.disabled = !smelter->enabled
	);
	if (start_pressed) smelter->enabled = true;
	if (stop_pressed) smelter->enabled = false;
}

static void dummy_windows() {
	smelter_ui(&smelter, &smelter_window);
}
//
//
// void smelter_toggle_ui() {
// 	smelter_window.visible = !smelter_window.visible;
// }
//
