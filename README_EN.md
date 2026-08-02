# Battleship

[Русский](README.md) | **English**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus)
![SFML](https://img.shields.io/badge/SFML-2.6.x-8CC445)
![Platform](https://img.shields.io/badge/platform-Windows-0078D4?logo=windows)

An educational implementation of the classic Battleship game against a
computer opponent. The game places both fleets automatically, displays two
10 × 10 boards, and passes the turn to the other side after a miss.

## Features

- standard ten-ship fleet with ships from one to four cells long;
- automatic ship placement without adjacent ships;
- mouse controls and visual feedback for hits, misses, and sunk ships;
- an AI mode that finishes off damaged ships;
- a probability map for choosing promising target cells;
- a five-second delay before the computer's turn;
- separate classes for the board, AI, and rendering.

## Controls

- **Left-click the right board** — fire at a cell;
- **Close the window** — quit the game.

The player keeps shooting after a hit. A miss passes the turn to the computer.
The game ends when either fleet is completely destroyed.

## Architecture

| File | Purpose |
|---|---|
| `Game.*` | game loop, input, and turn switching |
| `Board.*` | cells, ships, shots, and victory checks |
| `AIController.*` | computer-player strategy |
| `ProbabilityMap.*` | probability calculation for the remaining ships |
| `Renderer.*` | board and cell-state rendering |
| `GameConfig.h` | board size, window size, and fleet composition |

## Requirements

- Windows 10/11;
- Visual Studio 2022 with the **Desktop development with C++** workload;
- CMake 3.20 or newer for the alternative build;
- SFML 2.6.x for x64.

## Building with Visual Studio

The Visual Studio project currently expects SFML 2.6.1 at
`C:\IT\SFML-2.6.1-windows-vc17-64-bit\SFML-2.6.1`. If your library is
installed elsewhere, update the paths in the project properties:

1. Open `battleship.sln`.
2. Select **x64** and either `Debug` or `Release`.
3. Set your `SFML/include` and `SFML/lib` directories.
4. Build the solution with `Ctrl+Shift+B`.

## Building with CMake

```powershell
cmake -S . -B build -A x64 `
  -DSFML_DIR="C:/Libraries/SFML/lib/cmake/SFML"
cmake --build build --config Release
./build/Release/BattleshipGame.exe
```

The `arial.ttf` font is copied next to the executable automatically.

## Status

Working educational prototype. Good next steps include manual fleet placement,
a new-game button, AI difficulty levels, and online multiplayer.
