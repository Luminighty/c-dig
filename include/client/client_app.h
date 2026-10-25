#ifndef CLIENT_APP_H
#define CLIENT_APP_H

#include "input.h"
#include "player.h"
#include "ui.h"
#include <raylib.h>


typedef struct {
	PlayerController player;
	UI ui;
	Input input;

	RenderTexture2D render_target;
	RenderTexture2D ui_target;
} ClientApp;

extern ClientApp client_app;

void client_app_init();
void client_app_update();
void client_app_render();
void client_app_destroy();


#endif // CLIENT_APP_H
