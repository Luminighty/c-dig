#ifndef MATERIAL_H
#define MATERIAL_H


#define MATERIAL_TYPES \
	X(MATERIAL_DIRT, dirt)\
	X(MATERIAL_STONE, stone)\
	X(MATERIAL_COAL, coal)\
	X(MATERIAL_IRON, iron)\


typedef enum {
#define X(enum_variant, _) enum_variant,
	MATERIAL_TYPES
#undef X
	MATERIAL_SIZE,
} MaterialType;


typedef struct {
	int amounts[MATERIAL_SIZE];
	int full_amount;
} MaterialComposition;

void material_composition_merge_into(MaterialComposition* target, const MaterialComposition* right);
void material_composition_print(MaterialComposition* composition);


typedef struct {
#define X(_, name) int name;
	MATERIAL_TYPES
#undef X
} MaterialCompositionCreateParams;

MaterialComposition _material_composition_create(MaterialCompositionCreateParams params);

#define material_composition_create(...) _material_composition_create((MaterialCompositionCreateParams){__VA_ARGS__})


#endif // MATERIAL_H
