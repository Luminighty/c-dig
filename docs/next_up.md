# Next Up

I should look into the following features

## Misc

- [X] Make sprites flippable
- [ ] Digging should take time

## Project Structure

- [ ] Flat is getting a bit difficult to manage, and probably even more when it would come to onboarding
- [ ] Find a way to either update or generate makefile/lua build file


## AutoGenerate Asset Registry

- [X] Find files in assets
  - Will use `all.lua` to discover files
- [ ] Build a texture from filenames automatically

## Tiles as Config

- [X] Generate Tile data from lua configs
- [ ] Generate tile textures from templates
  - [ ] Texture Generator Script through raylib?
  - [X] Gather required texture templates
  - [ ] somehow tell generator script to generate my templates
    - stdio?
  - [ ] Better yet, if you manage to generate them through lua only without the need of C


## Light Level
 - [ ] store light level + sources
 - [ ] Flood fill light
 - [ ] Automatically light (mark dirty, update at end of frame)
 - [ ] Automatically dim


## UI research + init


## Lua Systems init


## Worldgen 1.0 init
 - Any ways to automatically include ores in worldgen
 - Biome definitions as config?


