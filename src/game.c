#include <stdlib.h>
#include "config.h"
#include "raylib.h"
#include "piece.h"
#include "game.h"
#include "time.h"
#include "utils.h"
#include "render.h"

void UpdateLevel(int * level, int comboCounter, float * speed) {

    for (int i = 0; i <= 200; i += 10) {

        if (comboCounter >= i) {
            
            *speed = 1 + ((i / 10) * 0.25);
            *level = 1 + (i / 10);
        }
    }
}

bool CheckButtonHover(Rectangle button) {

    Vector2 mouse = GetMousePosition();

    return CheckCollisionPointRec(mouse, button);
}

bool CheckButtonPress(Rectangle button) {

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, button)) {
        
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return true;
        }
    }

    return false;
}

void InitializeGame(Game_t *game) {
    
    game->head = malloc(sizeof(Block_t));
    game->head->next = NULL;
    
    for (int i = 0; i < GRID_HEIGHT; i++) {
    
        game->comboLines[i] = false;
    }

    game->isGameOver = false;
    game->gameStarted = false;

    game->score = 0;
    game->level = 1;
    game->comboCounter = 0;
    game->frame = 0;

    for (int i = 0; i < PIECE_COUNT; i++) {
    
        game->types[i] = i;
    }
    game->typeIndex = 0;

    game->currentPiece = SpawnPiece(game->types[game->typeIndex], (WINDOW_WIDTH / 2), GRID_STARTING_Y, 0);
    game->speed = 1.0f;
    game->skipEveryFrame = TARGET_FPS / game->speed;

    game->playButton.width = 300;
    game->playButton.height = 80;
    game->playButton.x = WINDOW_WIDTH / 2 - game->playButton.width / 2;
    game->playButton.y = WINDOW_HEIGHT / 2 - 300;
    
    srand(time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TETRIS");
    SetTargetFPS(TARGET_FPS);        
    
    ShuffleArray(game->types, PIECE_COUNT);
}

void GameInput(Game_t *game) {
    
    GetMovementInput(game->currentPiece, game->head);
}

void GameUpdate(Game_t *game) {

    while (!game->gameStarted) {

        if (CheckButtonPress(game->playButton)) {
            game->gameStarted = true;
        }
        else {return;}
    }

    if (game->level == 20) {game->isGameOver = true;}

    game->frame += 1;
    
    ApplyCollisionsBlocks(game->currentPiece, game->head);
    ApplyCollisionsWalls(game->currentPiece);
        
    if (game->frame >= game->skipEveryFrame) {
        
        game->frame = 0;
        
        CurrentPieceGravity(game->currentPiece);
        ApplyCollisionsBlocks(game->currentPiece, game->head);
        ApplyCollisionsWalls(game->currentPiece);
        
        if (game->currentPiece->isPlaced) {
            
            game->typeIndex += 1;
            
            if (game->typeIndex > 6) {
                
                game->typeIndex = 0;
                
                ShuffleArray(game->types, PIECE_COUNT);
            }
            
            SavePlacedBlocks(game->currentPiece, game->head);
            
            if (DetectCombos(game->head, game->comboLines, &game->comboCounter)) {
                
                LineExplosions(game->head, game->comboLines, &game->score, game->level);
                
                UpdateLevel(&game->level, game->comboCounter, &game->speed);
                
                game->skipEveryFrame = TARGET_FPS / game->speed;
            }
            
            if (IsGameOver(game->head)) {game->isGameOver = true;}
            
            game->currentPiece = SpawnPiece(game->types[game->typeIndex], (WINDOW_WIDTH / 2), GRID_STARTING_Y, 0);
        }
    }
}

void GameRender(Game_t *game) {
    
    BeginDrawing();

    if (game->isGameOver) {DrawGameOverMessage(game->score);}

    else if (!game->gameStarted) {
        
        DrawMainMenu(game->playButton);
        
        if (CheckButtonHover(game->playButton)) {

            DrawRectangleLinesEx(game->playButton, 5, DARKBLUE); 
        }
    }

    else {

        ClearBackground(DARKGRAY);
        DrawInfo(game->score, game->level, game->comboCounter);
        DrawTetrisGrid();
        DrawCurrentPiece(game->currentPiece);        
        DrawPlacedBlocks(game->head);   
    }    
    
    EndDrawing();   
}

void KillGame(Game_t *game) {

    CloseWindow();
    FreeMemory(game->head, game->currentPiece);
}
