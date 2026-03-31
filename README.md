# Fat Party

A multiplayer co-op dungeon action RPG built with Unreal Engine 5.1. Team up with friends via Steam to battle through dungeon levels as a Knight or Archer, fighting waves of undead enemies and bosses.

## Gameplay
- Two playable classes: Knight (melee) and Archer (ranged)
- Steam-integrated multiplayer with lobby system
- Enemy types: melee minions, skeleton archers, and boss encounters
- Hazards: spike traps, trap blades, turrets, moving platforms, jump pads
- Power-ups and quest items scattered across levels
- Rich dungeon environment with interactive decoration objects

## Project Structure
- `Source/FatParty/Characters/` — Knight and Archer player characters
- `Source/FatParty/BasicEnemies/` — base enemy and skeleton archer variants
- `Source/FatParty/AI_Components/` — behavior tree controllers for minions and boss
- `Source/FatParty/GameModes/` — lobby and gameplay game modes
- `Source/FatParty/Components/` — reusable components and movement systems
- `Source/FatParty/UI/` — HUD and menu system
- `Content/Blueprints/Actors/` — traps, projectiles, power-ups, decoration objects
- `Content/Maps/` — Level 01 and Level 02

## Built With
- Unreal Engine 5.1
- C++
- Steam OnlineSubsystem (multiplayer)
- Behavior Trees + EQS (AI)
