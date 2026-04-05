#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define TARGET_FPS 60

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT WINDOW_WIDTH * 2

#define GRID_SQUARE_SIZE 65

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define GRID_STARTING_X (WINDOW_WIDTH / 2) - (GRID_SQUARE_SIZE * GRID_WIDTH) / 2
#define GRID_STARTING_Y (WINDOW_HEIGHT / 2) - (GRID_SQUARE_SIZE * GRID_HEIGHT) / 2

#define GRID_WALL_RIGHT (GRID_STARTING_X + (GRID_WIDTH * GRID_SQUARE_SIZE))
#define GRID_FLOOR (GRID_STARTING_Y + (GRID_HEIGHT * GRID_SQUARE_SIZE))

enum piecetypes {
    
    I_SHAPED,
    J_SHAPED,
    L_SHAPED,
    O_SHAPED, 
    S_SHAPED,
    Z_SHAPED,
    T_SHAPED
};

typedef struct Block_t
{
    Rectangle rect;
    Color color;
    Color outlineColor;
    Vector2 lastPosition;
    struct Block_t * next;

} Block_t;

typedef struct Piece_t {

    int type;
    int x;
    int y;
    int rotation;
    Block_t blocks[4];
    bool isPlaced;

} Piece_t;

void ShuffleArray(int * array, int size) {
    
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void DrawTetrisGrid() {
    
    for (int i = 0; i < GRID_WIDTH; i++) {

        for (int j = 0; j < GRID_HEIGHT; j++) {

            DrawRectangle(GRID_STARTING_X + i * GRID_SQUARE_SIZE, GRID_STARTING_Y + j * GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, BLACK);

            DrawRectangleLines(GRID_STARTING_X + i * GRID_SQUARE_SIZE, GRID_STARTING_Y + j * GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, DARKGRAY);
        }
    }
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

Piece_t * SpawnPiece(int type, int x, int y, int rotation) {
    
    Piece_t * newPiece = malloc(sizeof(Piece_t));

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
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {-1, 1, 2};
            int offsetsY[3] = {0, 0, 0};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = BLUE,
                    .outlineColor = BLACK 
                };
            }
            break;
        }

        case J_SHAPED:
        {
            newPiece->blocks[3].color = DARKBLUE;
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {-1, -1, 1};
            int offsetsY[3] = {-1, 0, 0};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = DARKBLUE,
                    .outlineColor = BLACK 
                };
            }
            break;
        }

        case L_SHAPED:
        {
            newPiece->blocks[3].color = ORANGE;
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {-1, 1, 1};
            int offsetsY[3] = {0, 0, -1};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = ORANGE,
                    .outlineColor = BLACK 
                };
            }
            break;
        }

        case O_SHAPED:
        {
            newPiece->blocks[3].color = YELLOW;
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {0, 1, 1};
            int offsetsY[3] = {1, 0, 1};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = YELLOW,
                    .outlineColor = BLACK 
                };
            }
            break;
        }

        case S_SHAPED:
        {
            newPiece->blocks[3].color = GREEN;
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {-1, 0, 1};
            int offsetsY[3] = {1, 1, 0};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = GREEN,
                    .outlineColor = BLACK 
                };
            }
            break;
        }

        case Z_SHAPED:
        {
            newPiece->blocks[3].color = RED;
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {-1, 0, 1};
            int offsetsY[3] = {0, 1, 1};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = RED,
                    .outlineColor = BLACK 
                };
            }
            break;
        }

        case T_SHAPED:
        {
            newPiece->blocks[3].color = PURPLE;
            newPiece->blocks[3].outlineColor = BLACK;
            
            int offsetsX[3] = {0, -1, 1};
            int offsetsY[3] = {-1, 0, 0};
            
            for (int i = 0; i < 3; i++) {
                newPiece->blocks[i] = (Block_t) 
                {
                    .rect = {newPiece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        newPiece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE},
                    .color = PURPLE,
                    .outlineColor = BLACK
                };
            }
            break;
        }
        default:
            break;
    }

    puts("Piece spawned.");
    return newPiece;            
}

void SavePlacedBlocks(Piece_t * piece, Block_t * head) {
    
    Block_t * pBlock = head;
    
    for (int i = 0; i < 4; i++) {

        Block_t * newBlock = malloc(sizeof(Block_t));
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

void DrawPlacedBlocks(Block_t * head) {
    
    Block_t * pBlock = head->next;

    while (pBlock != NULL) {

        DrawRectangleRec(pBlock->rect, pBlock->color);
        
        DrawRectangleLines(pBlock->rect.x, pBlock->rect.y, 
        GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, pBlock->outlineColor);

        pBlock = pBlock->next;
    }
}

void ApplyCollisionsWalls(Piece_t * piece) {
    
    for (int i = 0; i < 4; i++) {
    
        if (piece->blocks[i].rect.y + GRID_SQUARE_SIZE >= GRID_FLOOR) {

            piece->isPlaced = true;

            float overlap = (piece->blocks[i].rect.y + GRID_SQUARE_SIZE - GRID_FLOOR);

            for (int j = 0; j < 4; j++) {piece->blocks[j].rect.y -= overlap;}
            return;
        }
    }

    for (int i = 0; i < 4; i++) {
        
        if (piece->blocks[i].rect.x + GRID_SQUARE_SIZE > GRID_WALL_RIGHT) {

            float overlap = (piece->blocks[i].rect.x + GRID_SQUARE_SIZE - GRID_WALL_RIGHT);

            for (int j = 0; j < 4; j++) {piece->blocks[j].rect.x -= overlap;}
            return;
        }
    }

    for (int i = 0; i < 4; i++) {
        
        if (piece->blocks[i].rect.x < GRID_STARTING_X) {

            float overlap = (GRID_STARTING_X - piece->blocks[i].rect.x);

            for (int j = 0; j < 4; j++) {piece->blocks[j].rect.x += overlap;}
            return;
        }
    }
}

void ApplyCollisionsBlocks(Piece_t * piece, Block_t * head) {  

    Block_t * pBlock = head->next;

    for (int i = 0; i < 4; i++) {
        
        pBlock = head->next;

        while (pBlock != NULL) {
         
            if ((piece->blocks[i].rect.x == pBlock->rect.x) 
                && (piece->blocks[i].rect.y == pBlock->rect.y)) {
                    
                if (piece->blocks[i].lastPosition.x < pBlock->rect.x) {

                    for (int k = 0; k < 4; k++) {piece->blocks[k].rect.x -= GRID_SQUARE_SIZE;}
                } 
                
                else if (piece->blocks[i].lastPosition.x > pBlock->rect.x) {

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

void RotatePiece(Piece_t * piece, Block_t * head) {

    Block_t *pBlock = head->next;

    bool leftBlocked = false;
    bool rightBlocked = false;

    if (head->next != NULL) {
        
        for (int i = 0; i < 4; i++) {
        
            while (pBlock != NULL) {
                
                if (pBlock->rect.y == piece->blocks[i].rect.y) {

                    if (pBlock->rect.x == piece->blocks[i].rect.x - GRID_SQUARE_SIZE) {
                        leftBlocked = true;
                    }

                    if (pBlock->rect.x == piece->blocks[i].rect.x + GRID_SQUARE_SIZE) {
                        rightBlocked = true;
                    }
                }
                pBlock = pBlock->next;
            }
        }
    }

    if (leftBlocked || rightBlocked) {
        return;
    }

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

                    piece->blocks[i].rect = (Rectangle) {
                
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }

            else if (piece->rotation == 90 || piece->rotation == 270) {    
            
                int offsetsX[3] = {0, 0, 0};
                int offsetsY[3] = {-1, 1, 2};
            
             for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }
        }
            break;

        case J_SHAPED:
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 

            if (piece->rotation == 0) {    
            
                int offsetsX[3] = {-1, -1, 1};
                int offsetsY[3] = {-1, 0, 0};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                    
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }

            else if (piece->rotation == 90) {    
            
                int offsetsX[3] = {0, 1, 0};
                int offsetsY[3] = {-1, -1, 1};
            
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }

            else if (piece->rotation == 180) {    
            
                int offsetsX[3] = {-1, 1, 1};
                int offsetsY[3] = {0, 0, 1};
            
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }

            else if (piece->rotation == 270) {    
            
                int offsetsX[3] = {0, 0, -1};
                int offsetsY[3] = {-1, 1, 1};
            
             for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }
        }
            break;

        case L_SHAPED: 
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 
            
            if (piece->rotation == 0) {    
                
                int offsetsX[3] = {-1, 1, 1};
                int offsetsY[3] = {0, 0, -1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                    
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }
            else if (piece->rotation == 90) {    
                
                int offsetsX[3] = {0, 0, 1};
                int offsetsY[3] = {-1, 1, 1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {
                    
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE};
                }
            }
            else if (piece->rotation == 180) {    
                
                int offsetsX[3] = {-1, -1, 1};
                int offsetsY[3] = {1, 0, 0};
                
                for (int i = 0; i < 3; i++) {

                piece->blocks[i].rect = (Rectangle) {
                
                    piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                    piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                    GRID_SQUARE_SIZE, 
                    GRID_SQUARE_SIZE};
                }
            }
            else if (piece->rotation == 270) {    
                
                int offsetsX[3] = {-1, 0, 0};
                int offsetsY[3] = {-1, -1, 1};
                
                for (int i = 0; i < 3; i++) {

                piece->blocks[i].rect = (Rectangle) {
                
                    piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                    piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                    GRID_SQUARE_SIZE, 
                    GRID_SQUARE_SIZE};
            }
        }
    }
            break;

        case O_SHAPED:
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 

            if (piece->rotation == 0 ||piece->rotation == 90 || piece->rotation == 180 || piece->rotation == 270) {
                
                int offsetsX[3] = {0, 1, 1};
                int offsetsY[3] = {1, 0, 1};

                for (int i = 0; i < 3; i++) {
                    
                    piece->blocks[i].rect = (Rectangle) {
                        
                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }
        }
            break;
        
        case S_SHAPED:
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 

            if (piece->rotation == 0 || piece->rotation == 180) {

                int offsetsX[3] = {-1, 0, 1};
                int offsetsY[3] = {1, 1, 0};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }

            else if (piece->rotation == 90 || piece->rotation == 270) {

                int offsetsX[3] = {0, 1, 1};
                int offsetsY[3] = {-1, 0, 1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }
        }
            break;

        case Z_SHAPED:
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 

            if (piece->rotation == 0 || piece->rotation == 180) {

                int offsetsX[3] = {-1, 0, 1};
                int offsetsY[3] = {0, 1, 1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }

            else if (piece->rotation == 90 || piece->rotation == 270) {
                
                int offsetsX[3] = {1, 1, 0};
                int offsetsY[3] = {0, -1, 1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }
        }
            break;  

        case T_SHAPED:
        {
            piece->blocks[3].rect.x = piece->x;
            piece->blocks[3].rect.y = piece->y; 

            if (piece->rotation == 0) {

                int offsetsX[3] = {0, -1, 1};
                int offsetsY[3] = {-1, 0, 0};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }

            else if (piece->rotation == 90) {

                int offsetsX[3] = {0, 1, 0};
                int offsetsY[3] = {-1, 0, 1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }
            else if (piece->rotation == 180) {

                int offsetsX[3] = {-1, 0, 1};
                int offsetsY[3] = {0, 1, 0};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
                    };
                }
            }
            
            else if (piece->rotation == 270) {
                
                int offsetsX[3] = {-1, 0, 0};
                int offsetsY[3] = {0, -1, 1};
                
                for (int i = 0; i < 3; i++) {

                    piece->blocks[i].rect = (Rectangle) {

                        piece->blocks[3].rect.x + (offsetsX[i] * GRID_SQUARE_SIZE), 
                        piece->blocks[3].rect.y + (offsetsY[i] * GRID_SQUARE_SIZE), 
                        GRID_SQUARE_SIZE, 
                        GRID_SQUARE_SIZE
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

void GetMovementInput(Piece_t * piece, Block_t * head) {
    
    Block_t * pBlock;

    for (int i = 0; i < 4; i++) {

        piece->blocks[i].lastPosition.x = piece->blocks[i].rect.x;
        piece->blocks[i].lastPosition.y = piece->blocks[i].rect.y;
    }

    if (piece->isPlaced == false) {
        
        if (IsKeyPressed(KEY_SPACE)) {
            
            if (head->next == NULL) {
                
                bool canMove = true;
                
                while (canMove) {
                
                    for (int i = 0; i < 4; i++) {
                
                        if (piece->blocks[i].rect.y == GRID_FLOOR - GRID_SQUARE_SIZE) {
                
                            canMove = false;
                            break;
                        } 
                    }

                    if (canMove == false) {break;}

                    for (int i = 0; i < 4; i++) {
                
                        piece->blocks[i].rect.y += GRID_SQUARE_SIZE;
                    } 

                    piece->y += GRID_SQUARE_SIZE;
                }
            }
            
            else {

                bool canMove = true;

                while (canMove) {
    
                    for (int i = 0; i < 4; i++) {
                        
                        pBlock = head->next;
                        
                        while (pBlock != NULL) {
                            
                            if (pBlock->rect.x == piece->blocks[i].rect.x) {
                                
                                if (pBlock->rect.y == piece->blocks[i].rect.y + GRID_SQUARE_SIZE) {
                                
                                    canMove = false;    
                                    break;
                                }
                            
                            }
                            pBlock = pBlock->next;
                        }
                    }
    
                    for (int i = 0; i < 4; i++) {
                        
                        if (piece->blocks[i].rect.y + GRID_SQUARE_SIZE == GRID_FLOOR) {
                            canMove = false;
                        }
                    }

                    if (canMove == true) {
    
                        for (int i = 0; i < 4; i++) {
    
                            piece->blocks[i].rect.y += GRID_SQUARE_SIZE;
                        }
                        piece->y += GRID_SQUARE_SIZE;
                    }
                }            
            }
        }

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            
            RotatePiece(piece, head);
            return;
        }

        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            
            for (int i = 0; i < 4; i++) {

                piece->blocks[i].rect.x += GRID_SQUARE_SIZE;
            }
            piece->x += GRID_SQUARE_SIZE;    

            return;
        }
        
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            
            for (int i = 0; i < 4; i++) {
            
                piece->blocks[i].rect.x -= GRID_SQUARE_SIZE;
            }
            piece->x -= GRID_SQUARE_SIZE;

            return;
        }
        
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            
            for (int i = 0; i < 4; i++) {

                piece->blocks[i].rect.y += GRID_SQUARE_SIZE;
            }
            piece->y += GRID_SQUARE_SIZE;
            
            return;
        }
    }
}

void CurrentPieceGravity(Piece_t * piece) {
    
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

void DrawCurrentPiece(Piece_t * piece) {
    
    for (int i = 0; i < 4; i++) {
        
        DrawRectangleRec(piece->blocks[i].rect, piece->blocks[i].color);
        
        DrawRectangleLines(piece->blocks[i].rect.x, piece->blocks[i].rect.y, 
            GRID_SQUARE_SIZE, GRID_SQUARE_SIZE, piece->blocks[i].outlineColor);
    }
}

bool DetectCombos(Block_t * head, bool * comboLines, int * comboCounter) {
    
    if (head == NULL) {return false;}

    bool returnValue = false;
    int counter;
    Block_t * pBlock = head->next;

    for (int i = 0; i < GRID_HEIGHT; i++) {

        counter = 0;
        pBlock = head->next;

        while (pBlock != NULL) {
            if (pBlock->rect.y == GRID_STARTING_Y + i * GRID_SQUARE_SIZE) {counter += 1;}
            pBlock = pBlock->next;
        }

        if (counter == GRID_WIDTH) {
            puts("Combo detected.");
            comboLines[i] = true;
            returnValue = true;
            *comboCounter += 1;
        }
    }
    return returnValue;
}

void LineExplosions(Block_t * head, bool * comboLines, int * score, int level) {

    if (head == NULL) {return;}
    
    Block_t * pBlock;
    Block_t * temp;

    int lastLine = 0;
    int comboCounter = 0;

    for (int i = 0; i < GRID_HEIGHT; i++) {
        
        if (comboLines[i] == true) {
            
            comboLines[i] = false;
            lastLine = i;
            comboCounter += 1;

            pBlock = head;
            while (pBlock->next != NULL) {

                if (pBlock->next->rect.y == GRID_STARTING_Y + i * GRID_SQUARE_SIZE) {
                
                    temp = pBlock->next;
                    pBlock->next = pBlock->next->next;
                    free(temp);            
                } 
                else {pBlock = pBlock->next;}
            }
        }
    }

    pBlock = head->next;
    while (pBlock != NULL) {

        if (pBlock->rect.y < GRID_STARTING_Y + lastLine * GRID_SQUARE_SIZE) {
        
            pBlock->rect.y += GRID_SQUARE_SIZE * comboCounter;
        }
        pBlock = pBlock->next;
    }

    *score += comboCounter * 300 * (level + 1);
}

bool IsGameOver(Block_t * head) {
    
    Block_t * pBlock = head->next;

    while (pBlock != NULL) {
        
        if (pBlock->rect.y < GRID_STARTING_Y) {return true;}
        pBlock = pBlock->next;
    }

    return false;
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

void UpdateLevel(int * level, int comboCounter, float * speed) {

    for (int i = 0; i <= 200; i += 10) {

        if (comboCounter >= i) {
            
            *speed = 1 + ((i / 10) * 0.25);
            *level = 1 + (i / 10);
        }
    }
}

void FreeMemory(Block_t * head, Piece_t * piece) {
    
    Block_t * temp;

    if (piece != NULL) {free(piece);}

    while (head != NULL) {

        temp = head;
        head = head->next;
        free(temp);
        puts("Block freed.");
    }

    free(head);
    puts("Head freed.");
}

int main(void) {

    Block_t * head = malloc(sizeof(Block_t));
    Piece_t * currentPiece;
    
    bool comboLines[GRID_HEIGHT] = {false};
    bool isGameOver = false;
    
    int score = 0;
    int level = 1;
    int comboCounter = 0;

    int frame = 0;

    int types[7] = {I_SHAPED, J_SHAPED, L_SHAPED, O_SHAPED, S_SHAPED, Z_SHAPED, T_SHAPED};
    int typeIndex = 0;
    
    float speed = 1;
    float skipEveryFrame = TARGET_FPS / speed;
    
    srand(time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TETRIS");
    SetTargetFPS(TARGET_FPS);        

    ShuffleArray(types, sizeof(types) / sizeof(types[0]));
    
    head->next = NULL;
    currentPiece = SpawnPiece(types[typeIndex], (WINDOW_WIDTH / 2), GRID_STARTING_Y, 0);
    
    //GAMELOOP
    while (!WindowShouldClose()) {

        //UPDATE     
        frame += 1;
        GetMovementInput(currentPiece, head);
        ApplyCollisionsBlocks(currentPiece, head);
        ApplyCollisionsWalls(currentPiece);

        if (frame >= skipEveryFrame) {
            
            frame = 0;
            
            CurrentPieceGravity(currentPiece);
            ApplyCollisionsBlocks(currentPiece, head);
            ApplyCollisionsWalls(currentPiece);
            
            if (currentPiece->isPlaced) {
                
                typeIndex += 1;
                
                if (typeIndex > 6) {
                    
                    typeIndex = 0;
                    
                    ShuffleArray(types, sizeof(types) / sizeof(types[0]));
                }
                
                SavePlacedBlocks(currentPiece, head);
                
                if (DetectCombos(head, comboLines, &comboCounter)) {
                    
                    LineExplosions(head, comboLines, &score, level);
                    
                    UpdateLevel(&level, comboCounter, &speed);
                    
                    skipEveryFrame = TARGET_FPS / speed;
                }
                
                
                if (IsGameOver(head)) {isGameOver = true;}
                
                currentPiece = SpawnPiece(types[typeIndex], (WINDOW_WIDTH / 2), GRID_STARTING_Y, 0);
            }
        }

        //DRAW
        BeginDrawing();

        if (isGameOver) {DrawGameOverMessage(score);}

        else {
            
            ClearBackground(DARKBLUE);
            DrawInfo(score, level, comboCounter);
            DrawTetrisGrid();
            DrawPlacedBlocks(head);
            DrawCurrentPiece(currentPiece);
        }

        EndDrawing();   
    }
    //DE-INITIALIZATION
    CloseWindow();
    FreeMemory(head, currentPiece);
    return 0;
}