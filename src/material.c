#include "material.h"
#include <stdio.h>


#define for_material(material) \
	for (int material = 0; material < MATERIAL_SIZE; material++)


static const char* MATERIAL_LABEL[] = {
	[MATERIAL_DIRT] = "DIRT",
	[MATERIAL_STONE] = "STONE",
	[MATERIAL_COAL] = "COAL",
	[MATERIAL_IRON] = "IRON",
};

void material_composition_merge_into(MaterialComposition* target, const MaterialComposition* right) {
	for_material(material) {
		target->amounts[material] += right->amounts[material];
	}
	target->full_amount += right->full_amount;
}


void material_composition_print(MaterialComposition* composition) {
	printf("sum => %d\n", composition->full_amount);
	for_material(material) {
		printf("%s => %d\n", MATERIAL_LABEL[material], composition->amounts[material]);
	}

}

inline MaterialComposition _material_composition_create(MaterialCompositionCreateParams params) {
	int sum = 0;
	MaterialComposition composition = {0};
#define X(enum, name) sum += composition.amounts[enum] = params. name;
	MATERIAL_TYPES
#undef X
	composition.full_amount = sum;
	return composition;
}
