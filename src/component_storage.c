#include "component_storage.h"
#include "components.h"
#include <stdarg.h>
#include <stdio.h>


ComponentBitmap _componentbitmap_create(ComponentType c, ...) {
    ComponentBitmap bitmap = {0};
    va_list args;
    for (va_start(args, c); c != COMPONENT_SIZE; c = va_arg(args, ComponentType)) {
        bitmap.bytes[c / COMPONENTBITMAP_SLOTSIZE] |= COMPONENT_TO_SLOTBIT(c);
    }
    return bitmap;
}

