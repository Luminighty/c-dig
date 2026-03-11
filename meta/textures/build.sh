#!/bin/bash

LDFLAGS="-Llibs -lraylib -lpthread -lm -ldl"
LDFLAGS_WIN="-lgdi32 -lwinmm"
LDFLAGS_UNIX="-lGL -ldl -lX11"

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    TARGET_FLAGS=$LDFLAGS_WIN
else
    TARGET_FLAGS=$LDFLAGS_UNIX
fi

gcc -o ./meta/textures/generator ./meta/textures/main.c $LDFLAGS $TARGET_FLAGS
