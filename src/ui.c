#include "ui.h"
#include "game.h"
#include "player.h"
#include <raylib.h>
#include <stdbool.h>


static char *slot_label[] = {
	[PLAYER_TOOL_PICKAXE] = "PICKAXE",
	[PLAYER_TOOL_WOOD] = "WOOD",
	[PLAYER_TOOL_STONE] = "STONE",
	[PLAYER_TOOL_TORCH] = "TORCH",
	[PLAYER_TOOL_CHEST] = "CHEST",
	[PLAYER_TOOL_FURNACE] = "FURNACE",
	[PLAYER_TOOL_WORKBENCH] = "WORKBENCH",
	[PLAYER_TOOL_ANVIL] = "ANVIL",
	[PLAYER_TOOL_LADDER] = "LADDER",
};


static inline void render_slots() {
	int slot = 0;
	for (int i = 0; i < PLAYER_TOOLBAR_SIZE; i++) {
		PlayerToolSlot *tool = &game.player.tools[i];
		if (tool->kind == PLAYER_TOOL_NONE)
			continue;
		bool is_selected = game.player.current_tool == i;
		if (is_selected) {
			const char *text = TextFormat("> %s %d", slot_label[tool->kind], tool->amount);
			DrawText(text, 5, slot * 12, 10, WHITE);
		} else {
			char *text = slot_label[tool->kind];
			DrawText(text, 5, slot * 12, 10, WHITE);
		}
		slot++;
	}
}


void ui_render(UI *ui) {
	render_slots();
}

