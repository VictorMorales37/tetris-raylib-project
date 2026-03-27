#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT WINDOW_WIDTH * 2
#define GRID_SQUARE_SIZE 65
#define TARGET_FPS 60
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define GRID_STARTING_X (WINDOW_WIDTH / 2) - (GRID_SQUARE_SIZE * GRID_WIDTH) / 2
#define GRID_STARTING_Y (WINDOW_HEIGHT / 2) - (GRID_SQUARE_SIZE * GRID_HEIGHT) / 2
#define GRID_WALL_RIGHT (GRID_STARTING_X + (GRID_WIDTH * GRID_SQUARE_SIZE))
#define GRID_FLOOR (GRID_STARTING_Y + (GRID_HEIGHT * GRID_SQUARE_SIZE))

enum pieceTypes {
    
    I_SHAPED,
    J_SHAPED,
    L_SHAPED,
    O_SHAPED, 
    S_SHAPED,
    Z_SHAPED,
    T_SHAPED
};

typedef struct Block
{
    Rectangle rect;
    Color color;
    Color outlineColor;
    Vector2 lastPosition;
    struct Block * next;

} Block;

typedef struct Piece {

    int type;
    int x;
    int y;
    int rotation;
    Block blocks[4];
    bool isPlaced;

} Piece;

void drawGrid() {
    
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            
            DrawRectangle(GRID_STARTING_X + i * GRID_SQUARE_SIZE, GRID_STARTING_Y + j * GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, BLACK);
            
            DrawRectangleLines(GRID_STARTING_X + i * GRID_SQUARE_SIZE, GRID_STARTING_Y + j * GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, DARKGRAY);
        }
    }
}   

Piece * spawnPiece(int type, int x, int y, int rotation) {
    
    Piece * newPiece = malloc(sizeof(Piece));

    newPiece->x = x;
    newPiece->y = y;

    newPiece->isPlaced = false;
    newPiece->type = type;
    newPiece->rotation = rotation;
    
    newPiece->blocks[3].rect = (Rectangle) {x, y, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE};

    switch (newPiece->type) {
        case I_SHAPED:
        {
            newPiece->blocks[3].color = BLUE;
            newPiece->blocks[3].outlineColor = DARKBLUE;

            if (newPiece->rotation == 0 || newPiece->rotation == 180) {    
            
                int offsetsX[3] = {-1, 1, 2};
                int offsetsY[3] = {0, 0, 0};
                
                for (int i = 0; i < 3; i++) {
                    newPiece->blocks[i] = (Block) 
                    {
                        .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                            newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                            GRID_SQUARE_SIZE, 
                            GRID_SQUARE_SIZE},
                        .color = BLUE,
                        .outlineColor = DARKBLUE 
                    };
                }
            }

            else if (newPiece->rotation == 90 || newPiece->rotation == 270) {    
            
                int offsetsX[3] = {0, 0, 0};
                int offsetsY[3] = {-1, 1, 2};
            
                for (int i = 0; i < 3; i++) {
                    newPiece->blocks[i] = (Block) 
                    {
                        .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                            newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                            GRID_SQUARE_SIZE, 
                            GRID_SQUARE_SIZE},
                        .color = BLUE,
                        .outlineColor = DARKBLUE 
                    };
                }
            }
        break;
        }

        default:
            break;
    }

    puts("New piece spawned");
    return newPiece;            
}

void savePlacedBlocks(Piece * piece, Block * head) {
    
    Block * pBlock = head;
    
    for (int i = 0; i < 4; i++) {
        
        Block * newBlock = malloc(sizeof(Block));
        newBlock->next = NULL;

        newBlock->rect = piece->blocks[i].rect;
        newBlock->color = piece->blocks[i].color;
        newBlock->outlineColor = piece->blocks[i].outlineColor;
        newBlock->lastPosition = piece->blocks[i].lastPosition;

        while (pBlock->next != NULL) {
            pBlock = pBlock->next;
        }

        pBlock->next = newBlock;
        puts("Saved 1 block.");
    }

    free(piece);
    puts("Freed piece.");
}

void drawPlacedBlocks(Block * head) {
    
    Block * pBlock = head->next;

    while (pBlock != NULL) {
        DrawRectangleRec(pBlock->rect, pBlock->color);
        DrawRectangleLines(pBlock->rect.x, pBlock->rect.y, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, pBlock->outlineColor);

        pBlock = pBlock->next;
    }
}

void applyCollisionsWalls(Piece * piece) {
    
    for (int i = 0; i < 4; i++) {
    
        if (piece->blocks[i].rect.y + GRID_SQUARE_SIZE >= GRID_FLOOR) {

            piece->isPlaced = true;
            float overlap = (piece->blocks[i].rect.y + GRID_SQUARE_SIZE - GRID_FLOOR);

            for (int j = 0; j < 4; j++) {
                piece->blocks[j].rect.y -= overlap;
            }
            return;
        }
    }

    for (int i = 0; i < 4; i++) {
        
        if (piece->blocks[i].rect.x + GRID_SQUARE_SIZE > GRID_WALL_RIGHT) {

            float overlap = (piece->blocks[i].rect.x + GRID_SQUARE_SIZE - GRID_WALL_RIGHT);

            for (int j = 0; j < 4; j++) {
                piece->blocks[j].rect.x -= overlap;
            }
            return;
        }
    }

    for (int i = 0; i < 4; i++) {
        
        if (piece->blocks[i].rect.x < GRID_STARTING_X) {

            float overlap = (GRID_STARTING_X - piece->blocks[i].rect.x);

            for (int j = 0; j < 4; j++) {
                piece->blocks[j].rect.x += overlap;
            }
            return;
        }
    }

}

void applyCollisionsBlocks(Piece * piece, Block * head) {  

    Block * pBlock = head->next;

    for (int i = 0; i < 4; i++) {
        
        pBlock = head->next;

        while (pBlock != NULL) {
         
            if ((piece->blocks[i].rect.x == pBlock->rect.x) 
            && (piece->blocks[i].rect.y == pBlock->rect.y)) {
                
                if (piece->blocks[i].lastPosition.x < pBlock->rect.x) {
                    for (int k = 0; k < 4; k++) {piece->blocks[k].rect.x -= GRID_SQUARE_SIZE;}
                }
                else if (piece->blocks[i].lastPosition.x > pBlock->rect.x){
                    for (int k = 0; k < 4; k++) {piece->blocks[k].rect.x += GRID_SQUARE_SIZE;}
                }

                if (piece->blocks[i].lastPosition.y < pBlock->rect.y) {
                    for (int k = 0; k < 4; k++) {piece->blocks[k].rect.y -= GRID_SQUARE_SIZE;}
                    piece->isPlaced = true;
                    return;
                }
                else if (piece->blocks[i].lastPosition.y > pBlock->rect.y){
                    for (int k = 0; k < 4; k++) {piece->blocks[k].rect.y += GRID_SQUARE_SIZE;}
                }
            }

            pBlock = pBlock->next;
        }
    }
}

void rotatePiece(Piece * piece) {

    piece->rotation = (piece->rotation + 90) % 360;

    switch(piece->type) {

        case I_SHAPED:
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 

            if (piece->rotation == 0 || piece->rotation == 180) {    
            
                int offsetsX[3] = {-1, 1, 2};
                int offsetsY[3] = {0, 0, 0};
                
                for (int i = 0; i < 3; i++) {
                    piece->blocks[i] = (Block) 
                    {
                        .rect = {piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                            piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                            GRID_SQUARE_SIZE, 
                            GRID_SQUARE_SIZE},
                        .color = BLUE,
                        .outlineColor = DARKBLUE 
                    };
                }
            }

            else if (piece->rotation == 90 || piece->rotation == 270) {    
            
                int offsetsX[3] = {0, 0, 0};
                int offsetsY[3] = {-1, 1, 2};
            
                for (int i = 0; i < 3; i++) {
                    piece->blocks[i] = (Block) 
                    {
                        .rect = {piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                            piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                            GRID_SQUARE_SIZE, 
                            GRID_SQUARE_SIZE},
                        .color = BLUE,
                        .outlineColor = DARKBLUE 
                    };
                }
            }
        }
        break;

        default: 
            break;
    }
    return;
}

void movementInput(Piece * piece) {
    
    for (int i = 0; i < 4; i++) {
        piece->blocks[i].lastPosition.x = piece->blocks[i].rect.x;
        piece->blocks[i].lastPosition.y = piece->blocks[i].rect.y;
    }

    if (piece->isPlaced == false) {
        
        if (IsKeyPressed(KEY_W)) {
            
            rotatePiece(piece);
            return;
        }

        if (IsKeyPressed(KEY_D)) {
            
            for (int i = 0; i < 4; i++) {
                piece->blocks[i].rect.x += GRID_SQUARE_SIZE;
            }
            piece->x += GRID_SQUARE_SIZE;    

            if (piece->isPlaced) {return;}
            return;
        }
        
        if (IsKeyPressed(KEY_A)) {
            
            for (int i = 0; i < 4; i++) {
                piece->blocks[i].rect.x -= GRID_SQUARE_SIZE;
            }
            piece->x -= GRID_SQUARE_SIZE;

            if (piece->isPlaced) {return;}
            return;
        }
        
        if (IsKeyPressed(KEY_S)) {
            
            for (int i = 0; i < 4; i++) {
                piece->blocks[i].rect.y += GRID_SQUARE_SIZE;
            }
            piece->y += GRID_SQUARE_SIZE;
            
            if (piece->isPlaced) {return;}
            return;
        }
    }
}

void movePieceDown(Piece * piece) {
    
    for (int i = 0; i < 4; i++) {
        piece->blocks[i].lastPosition.x = piece->blocks[i].rect.x;
        piece->blocks[i].lastPosition.y = piece->blocks[i].rect.y;
    }

    if (piece->isPlaced == false) {
        
        for (int i = 0; i < 4; i++) {        
            piece->blocks[i].rect.y += GRID_SQUARE_SIZE;
        }
        piece->y += GRID_SQUARE_SIZE;
    }
}

void drawPiece(Piece * piece) {
    
    for (int i = 0; i < 4; i++) {
        DrawRectangleRec(piece->blocks[i].rect, piece->blocks[i].color);
        DrawRectangleLines(piece->blocks[i].rect.x, piece->blocks[i].rect.y, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, piece->blocks[i].outlineColor);
    }
}

bool detectCombos(Block * head, bool * comboLines) {
    
    if (head == NULL) {return false;}

    int counter = 0;
    int comboCounter = 0;
    Block * pBlock = head->next;

    for (int i = 0; i < GRID_HEIGHT; i++) {

        counter = 0;
        pBlock = head->next;

        while (head->next != NULL && pBlock != NULL) {
            if (pBlock->rect.y == GRID_STARTING_Y + i * GRID_SQUARE_SIZE) {
                counter += 1;
            }
            pBlock = pBlock->next;
        }

        if (counter == GRID_WIDTH) {
            comboCounter += 1;
            comboLines[i] = true;
        }
    }

    if (counter >= GRID_WIDTH) {
        printf("\n%d combos detected\n", comboCounter);
        return true;
    }

    return false;
}

int main(void) {
    
    Block * head = malloc(sizeof(Block));
    head->next = NULL;
    
    bool comboLines[GRID_HEIGHT] = {false};
    float speed = 1;
    float skipEveryFrame = TARGET_FPS / speed;
    int frame = 0;
    
    Piece * currentPiece;
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TETRIS");
    SetTargetFPS(TARGET_FPS);        
    
    currentPiece = spawnPiece(I_SHAPED, (WINDOW_WIDTH / 2), GRID_STARTING_Y, 0);
    
    //GAMELOOP
    while (!WindowShouldClose()) {
        
        //UPDATE     
        frame += 1;
        movementInput(currentPiece);
        applyCollisionsBlocks(currentPiece, head);
        applyCollisionsWalls(currentPiece);
        
        if (frame >= skipEveryFrame) {
            
            frame = 0;
            
            movePieceDown(currentPiece);
            applyCollisionsBlocks(currentPiece, head);
            applyCollisionsWalls(currentPiece);
            
            if (currentPiece->isPlaced) {
                savePlacedBlocks(currentPiece, head);
                if (detectCombos(head, comboLines)) {
                }
                currentPiece = spawnPiece(I_SHAPED, (WINDOW_WIDTH / 2), GRID_STARTING_Y, 0);
            }
        }

        //DRAW
        BeginDrawing();

        ClearBackground(DARKPURPLE);
        drawGrid();
        drawPlacedBlocks(head);
        drawPiece(currentPiece);

        EndDrawing();   

    }
    //DE-INITIALIZATION
    CloseWindow();
    Block * temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
        puts("Block freed");
    }
    free(head);
    puts("Head freed");
    return 0;
}