#include <string.h>
#include "render.h"
#include "raylib.h"
#include "config.h"

void DrawMainMenu(Rectangle playButton) {

    ClearBackground(DARKPURPLE);
    
    DrawText("TETRIS", (WINDOW_WIDTH / 2) - (MeasureText("TETRIS", 150) / 2), 150, 150, WHITE);

    DrawRectangleRec(playButton, WHITE);
    
    DrawText("Play", WINDOW_WIDTH / 2 - MeasureText("Play", 50) / 2, 
    playButton.y + playButton.height / 4, 50, DARKBLUE);
}

void DrawInfo(int score, int level, int lineCounter) {
    
    char scoreText[20] = "Score: ";
    char levelText[20] = "Level: ";
    char linesText[20] = "Lines: ";

    char scoreValue[6] = "";
    char levelValue[2] = "";
    char linesValue[6] = "";

    sprintf(scoreValue, "%d", score);
    sprintf(levelValue, "%d", level);
    sprintf(linesValue, "%d", lineCounter);
    
    strcat(scoreText, scoreValue);
    strcat(levelText, levelValue);
    strcat(linesText, linesValue);

    DrawText(scoreText, 50, 10, 50, WHITE);
    DrawText(levelText , 50, 50, 50, WHITE);
    DrawText(linesText , 50, 90, 50, WHITE);
}

void DrawGameOverMessage(int score) {

    char scoreString[30] = "Score: "; 
    char scoreValue[20] = "";

    sprintf(scoreValue, "%d", score);
    strcat(scoreString, scoreValue);

    ClearBackground(DARKBLUE);
    DrawText("Game Over", (WINDOW_WIDTH / 2) - MeasureText("Game Over", 100) / 2, 
    WINDOW_HEIGHT / 2, 100, WHITE);

    DrawText(scoreString, (WINDOW_WIDTH / 2) - MeasureText(scoreString, 50) / 2, 
    WINDOW_HEIGHT / 2 + 200, 50, WHITE);
}

void DrawTetrisGrid() {
    
    for (int i = 0; i < GRID_WIDTH; i++) {

        for (int j = 0; j < GRID_HEIGHT; j++) {

            DrawRectangle(GRID_STARTING_X + i * GRID_SQUARE_SIZE, GRID_STARTING_Y + j * GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, BLACK);

            DrawRectangleLines(GRID_STARTING_X + i * GRID_SQUARE_SIZE, GRID_STARTING_Y + j * GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, DARKGRAY);
        }
    }
} 

void DrawCurrentPiece(Piece_t * piece) {
    
    for (int i = 0; i < 4; i++) {
        
        DrawRectangleRec(piece->blocks[i].rect, piece->blocks[i].color);
        
        DrawRectangleLines(piece->blocks[i].rect.x, piece->blocks[i].rect.y, 
            GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, piece->blocks[i].outlineColor);
        }
    }
    
void DrawPlacedBlocks(Block_t * head) {
    
    Block_t * pBlock = head->next;

    while (pBlock != NULL) {

        DrawRectangleRec(pBlock->rect, pBlock->color);
        
        DrawRectangleLines(pBlock->rect.x, pBlock->rect.y, 
        GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, pBlock->outlineColor);

        pBlock = pBlock->next;
    }
}    