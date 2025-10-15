#ifndef ASSETS_H
#define ASSETS_H

#include "assets_registry.h"
#include <raylib.h>


typedef struct {
	Texture2D textures[TEXTURE_SIZE];
} AssetServer;


void assets_init();
void assets_destroy();

Texture2D assets_texture(TextureId texture);

#endif // ASSETS_H
