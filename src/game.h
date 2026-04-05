#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "raylib.h"

typedef struct Game_t {

    Block_t *head;
    Piece_t *currentPiece;
    bool comboLines[GRID_HEIGHT];
    bool isGameOver;
    bool gameStarted;
    int score;
    int level;
    int comboCounter;
    int frame;
    int types[7];
    int typeIndex;
    float speed;
    float skipEveryFrame;
    Rectangle playButton;
} Game_t;

void InitializeGame(Game_t *game);

void GameInput(Game_t *game);
void GameUpdate(Game_t *game);
void GameRender(Game_t *game);

void KillGame(Game_t *game);

void UpdateLevel(int *level, int comboCounter, float *speed);
bool CheckButtonPress(Rectangle button);
bool CheckButtonHover(Rectangle button);

#endif