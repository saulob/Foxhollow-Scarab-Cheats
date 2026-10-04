# Foxhollow Scarab Cheat

A native mod that adds a simple Scarab currency cheat to Star Fox Adventures running through Foxhollow.

## Controls

| Key | Action |
| --- | --- |
| + | Add 10 Scarabs |

- The + key works on both the main keyboard and the numeric keypad. On keyboards where + shares the = key, the main key works with or without Shift. Numpad + works with Num Lock on or off.
- On Linux and macOS, the main key is recognized by its position: the key left of Backspace, = and + on a US layout. If that key types something else on your layout, use numpad +.
- Each press adds 10 Scarabs once. Holding + does not keep adding Scarabs; release it and press it again to add another 10. Both + keys count as one: pressing the other + key while one is held does not add more.
- The key works during gameplay while playing as Fox and the Foxhollow window has keyboard focus. It does nothing on the title screen, the save select, during loading, while flying the Arwing or while playing as Krystal. A key pressed while the window is in the background is ignored, not saved for later.
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
      linux-amd64/
        mod.so
      linux-arm64/
        mod.so
      macos-x86_64/
        mod.so
      macos-arm64/
        mod.so
```

Foxhollow only loads the library in the folder that matches your system and ignores the others, so you only need the folder for your platform.

Restart the game after installing.

## Platform support

| Platform | Folder | Status |
| --- | --- | --- |
| Windows x64 | `windows-amd64` | Tested in game |
| Linux x86_64 | `linux-amd64` | Build validated, in-game testing pending |
| Linux ARM64 | `linux-arm64` | Build validated by GitHub Actions, in-game testing pending |
| macOS Apple Silicon | `macos-arm64` | Build validated by GitHub Actions, in-game testing pending |
| macOS Intel | `macos-x86_64` | Build validated by GitHub Actions, in-game testing pending |

Official Foxhollow builds are currently published for Windows x64, Linux x86_64 and macOS Apple Silicon. The Linux ARM64 and macOS Intel libraries are for Foxhollow builds you compile yourself. Windows on ARM is not supported.

On Linux and macOS, the + keys are read from the keyboard state of Foxhollow's own SDL3 runtime, so the mod needs no extra libraries and behaves the same under X11 and Wayland. If the Foxhollow log shows `[Scarab Cheat] disabled: ...`, the mod could not find the game functions or keyboard input it needs and left the game unchanged.

## Repository

https://github.com/saulob/Foxhollow-Scarab-Cheats

## License

[MIT](LICENSE)
