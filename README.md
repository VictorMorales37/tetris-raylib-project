# tetris-raylib-project

To compile and run, follow these steps:

- Install raylib here: https://github.com/raysan5/raylib/releases
- After raylib setup, you can compile the source code with: gcc tetris.c -o tetris -lraylib -lm -ldl -lpthread -lGL -lX11

Tetris game recreation in C using the Raylib framework/library.

Current features:

- Piece spawning system
- Rotation movement
- Vertical movement (by gravity and player input) 
- Horizontal movement
- Piece_t placing
- Piece_t collisions
- Line combos
- "Game Over" possibility

Missing features:

- Main menu
- Settings menu
- Tetris' Signature random piece spawning
- Scoring system
- Music 