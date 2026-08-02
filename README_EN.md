# Console Snake with PDCurses

[Русский](README.md) | **English**

![C++](https://img.shields.io/badge/C%2B%2B-console-00599C?logo=cplusplus)
![Library](https://img.shields.io/badge/library-PDCurses-4B8BBE)
![Platform](https://img.shields.io/badge/platform-Windows-0078D4?logo=windows)

A colorful console version of the classic Snake game. It uses PDCurses for
cursor positioning, arrow-key input, and colored terminal output.

## Rules

Control the snake, collect `@` apples, and grow its tail. The game ends when
the snake hits the border or its own body.

Symbols:

- `$` — snake head;
- `#` — tail;
- `@` — apple;
- `*` — board border.

## Controls

- **Arrow keys** — change direction;
- **Q** — quit;
- after defeat, **N** — start again;
- after defeat, **Q** — exit the game.

An immediate turn in the opposite direction is blocked so that the snake
cannot collide with itself in a single frame.

## Technical Details

- head and tail coordinates are stored in `vector2di`;
- the tail is represented by a `std::vector` of coordinates;
- apples appear in random free cells;
- `timeout()` controls the update rate;
- vertical and horizontal movement use separate frame delays;
- the interface uses five PDCurses color pairs.

## Building

The repository already contains the `curses.h` and `panel.h` headers and the
`pdcurses.lib` library.

1. Open `MyNcurses.sln` in Visual Studio 2022.
2. Select the desired project platform.
3. Add `$(ProjectDir)` to the C/C++ include directories.
4. Add `$(ProjectDir)` to the linker library directories.
5. Verify that `pdcurses.lib` is listed as a dependency.
6. Build and run the console application.

> The current `.vcxproj` contains an absolute path from another computer.
> Replace it with `$(ProjectDir)` as described above.

## Status

Working educational prototype. Possible next steps include a high-score table,
gradual speed increases, a difficulty menu, and a CMake build.
