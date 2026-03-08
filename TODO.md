# TODO

Just a long list of things that needs to be done with varying difficulties.
Might make sense to open tickets later, organize them by difficulty, priority, ect.


- [ ] Make sprites flippable
- [ ] Digging should take time
- [ ] Reorganize project structure
- [ ] Lua as Systems
- [ ] Multiplayer (lot of effort, requires server)
- [ ] Load Textures from lua (generate them)
- [ ] Tiles as lua config
- [ ] Content Generation through lua
  - [ ] Generate Textures using colors+templates
  - [ ] Define material with tags/config/colors -> generate tile+material+texture+items
- [ ] Better build system
  - Find dependencies automaticall,
  - Maybe lua based,
  - Probably no need for incremental builds
- [ ] Worldgen
  - For now just make it a little bit more interesting
  - Caves,
  - ore clusters
  - Mushroom
- [ ] UI
  - This is an important one,
  - Investigate UI libraries, or how we could make our own
  - Perhaps lua ui components/configs?
  - Should be `dialog` based if possible
    - kinda like mmos, with billions of dialogs for each thing
    - this would let us make it composable
- [ ] Simulation
  - Falling saaaand
  - Liquids
  - Gas :)
- [ ] Survival
  - Inventory System (toolbelt + bag/backpack)
  - Tag Based Tools (Handle, Head, ect. any materials)
  - Rubble as quantity in a bag
  - Smelting using % of materials for different ingots
    - Steel requiring 70% iron + 30% carbon, ect.
  - Health/Injuries/Effects to change capabilities
  - Healing/Food
- [ ] Light Level System
  - Similar to Mc, probably each tile stores its light level
  - Apply it during rendering as a pass?
- [ ] Content Hill
  - More Ores
  - More Gems
  - Tools
  - Building Blocks
  - Workbenches
  - Mushroom Tree GROwing
  - Biomes
  - Explosives
  - More Explosives
    - Random chance for duds or giant explosions
  - Torches
- [ ] Contributing guide
  - Standardize code quality/naming
  - Allow different levels of lua contribution
    - Simple new content addition (e.g. new ore, items should be easy)
    - Lua Systems for more in-depth features
    - C for performance heavy/engine contributions
  - Write guide for each level
  - Have some low-hanging fruit tickets


