#ifndef ASSETS_REGISTRY_H
#define ASSETS_REGISTRY_H


#define REGISTRY_TEXTURES\
	TEXTURE(TILE_DIRT, "./assets/textures/tiles/dirt.png")\
	TEXTURE(TILE_GRASS, "./assets/textures/tiles/grass.png")\
	TEXTURE(TILE_STONE, "./assets/textures/tiles/stone.png")\
	TEXTURE(TILE_MOSS, "./assets/textures/tiles/moss.png")\
	TEXTURE(TILE_COAL, "./assets/textures/tiles/coal.png")\
	TEXTURE(TILE_IRON, "./assets/textures/tiles/iron.png")\
	TEXTURE(TILE_DIAMOND, "./assets/textures/tiles/diamond.png")\
	TEXTURE(TILE_AMETHYST, "./assets/textures/tiles/amethyst.png")\
	TEXTURE(TILE_EMERALD, "./assets/textures/tiles/emerald.png")\
	TEXTURE(TILE_RUBY, "./assets/textures/tiles/ruby.png")\
	\
	TEXTURE(MINER_BEARD_00, "./assets/textures/miner/beard/0000.png")\
	TEXTURE(MINER_BEARD_01, "./assets/textures/miner/beard/0001.png")\
	TEXTURE(MINER_BEARD_02, "./assets/textures/miner/beard/0002.png")\
	TEXTURE(MINER_BEARD_03, "./assets/textures/miner/beard/0003.png")\
	TEXTURE(MINER_BEARD_04, "./assets/textures/miner/beard/0004.png")\
	TEXTURE(MINER_BEARD_05, "./assets/textures/miner/beard/0005.png")\
	TEXTURE(MINER_BODY_00, "./assets/textures/miner/body/0000.png")\
	TEXTURE(MINER_BODY_01, "./assets/textures/miner/body/0001.png")\
	TEXTURE(MINER_BODY_02, "./assets/textures/miner/body/0002.png")\
	TEXTURE(MINER_BODY_03, "./assets/textures/miner/body/0003.png")\
	TEXTURE(MINER_BODY_04, "./assets/textures/miner/body/0004.png")\
	TEXTURE(MINER_BODY_05, "./assets/textures/miner/body/0005.png")\
	TEXTURE(MINER_CLOTH_00, "./assets/textures/miner/clothes/0000.png")\
	TEXTURE(MINER_CLOTH_01, "./assets/textures/miner/clothes/0001.png")\
	TEXTURE(MINER_CLOTH_02, "./assets/textures/miner/clothes/0002.png")\
	TEXTURE(MINER_CLOTH_03, "./assets/textures/miner/clothes/0003.png")\
	TEXTURE(MINER_CLOTH_04, "./assets/textures/miner/clothes/0004.png")\
	TEXTURE(MINER_CLOTH_05, "./assets/textures/miner/clothes/0005.png")\
	TEXTURE(MINER_HAT_00, "./assets/textures/miner/hat/0000.png")\
	TEXTURE(MINER_HAT_01, "./assets/textures/miner/hat/0001.png")\
	TEXTURE(MINER_HAT_02, "./assets/textures/miner/hat/0002.png")\
	TEXTURE(MINER_HAT_03, "./assets/textures/miner/hat/0003.png")\
	TEXTURE(MINER_HAT_04, "./assets/textures/miner/hat/0004.png")\
	TEXTURE(MINER_HAT_05, "./assets/textures/miner/hat/0005.png")


typedef enum {
	TEXTURE_NONE,

	#define TEXTURE(id, _) TEXTURE_##id,
	REGISTRY_TEXTURES
	#undef TEXTURE

	TEXTURE_SIZE,
} TextureId;

#endif // ASSETS_REGISTRY_H
