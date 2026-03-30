# tetris-raylib-project

To compile, run "gcc tetris.c -o tetris -lraylib -lm -ldl -lpthread -lGL -lX11"

Tetris game recreation in C using the Raylib framework/library.

Current features:

- Piece_t spawning system
- Rotation movement
- Vertical movement (by gravity and player input) 
- Horizontal movement
- Piece_t placing
- Piece_t collisions
- Line combos

Missing features:

- Main menu
- Settings menu
- Other 5 piece types
- Tetris' Signature random piece spawning
- Scoring system
- "Game Over" possibility