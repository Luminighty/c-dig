#include "assets.h"
#include "assets_registry.h"
#include <raylib.h>

static AssetServer assets = {0};


void assets_init() {
	#define TEXTURE(enum, path) assets.textures[TEXTURE_##enum] = LoadTexture(path);
	REGISTRY_TEXTURES
	#undef TEXTURE
}


void assets_destroy() {
	#define TEXTURE(enum, path) UnloadTexture(assets.textures[TEXTURE_##enum]);
	REGISTRY_TEXTURES
	#undef TEXTURE
}


inline Texture2D assets_texture(TextureId texture) {
	return assets.textures[texture];
}
