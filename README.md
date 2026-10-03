# Foxhollow Scarab Cheat

A native mod that adds a simple Scarab currency cheat to Star Fox Adventures running through Foxhollow.

## Controls

| Key | Action |
| --- | --- |
| + | Add 10 Scarabs |

- The + key works on both the main keyboard and the numeric keypad. On keyboards where + shares the = key, the main key works with or without Shift.
- Each press adds 10 Scarabs once. Holding + does not keep adding Scarabs; release it and press it again to add another 10. Pressing the other + key while one is held does not add more.
- The key works during gameplay while playing as Fox and the game window is focused. It does nothing on the title screen, the save select, during loading, while flying the Arwing or while playing as Krystal.
- There is no on-screen display. Every addition is written to the Foxhollow log, for example `[Scarab Cheat] Added 10 Scarabs (20 / 50)`.

## Features

- **Add 10 Scarabs**: adds 10 Scarabs to Fox's current total. If the current Scarab Bag is too small for the new amount, the mod automatically upgrades it to the next normal capacity tier: 10 -> 50 -> 100 -> 200.
  - The bag is only upgraded when needed, one tier at a time.
  - The bag is never reset or downgraded.
  - Capacity never exceeds 200. With the 200 bag, your total stops at 200, as it does in the normal game.

The mod only changes the game when you press +. Without a key press, gameplay is unchanged.

## Installation

**Recommended:** install through the Foxhollow Launcher once the mod is published there.

**Manual:** place the extracted mod folder in the Foxhollow Launcher's `mods` folder, so it looks like this:

```
mods/
  com.saulob.cheats-scarab/
    mod.json
    lib/
      windows-amd64/
        mod.dll
```

Restart the game after installing.

## Platform support

- Windows x64

Other Foxhollow platforms are not supported by this mod yet.

## Repository

https://github.com/saulob/Foxhollow-Scarab-Cheats

## License

[MIT](LICENSE)
