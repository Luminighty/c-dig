#!/bin/bash

lua ./meta/components/generate.lua
lua ./meta/math/generate.lua

make
