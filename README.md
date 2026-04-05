# Raylib Tetris Project
This project consists of a Tetris clone built with C and the Raylib framework.

## Features
- UI and Visuals Rendering with Raylib
- Game Menu (Not finished)
- Piece Movement (Arrow keys, WASD)
- Piece Placing (Space key)
- Collisions
- Line Combos
- TETRIS Style Piece Shuffling
- Scores and Difficulty Progression

## To-do
- Placement Preview, a.k.a. "Ghost Pieces"
- "Next Piece" Preview
- Reset/Restart Button
  
## How to run
To compile and run, follow these steps:

### Linux
Step 1: Install GCC if not already installed.  
Step 2: Run: 
```bash
    make
    ./output/main
```

### Windows
Step 1: Install MinGW if not already installed.
Setp 2: Open cmd or PowerShell in the project folder and run:

```bash
    mingw32-make
    tetris.exe
```