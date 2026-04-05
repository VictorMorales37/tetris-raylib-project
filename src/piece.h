#ifndef PIECE_H
#define PIECE_H

#include "raylib.h"
#include <stdbool.h>

enum piecetypes {
    
    I_SHAPED,
    J_SHAPED,
    L_SHAPED,
    O_SHAPED, 
    S_SHAPED,
    Z_SHAPED,
    T_SHAPED,
    PIECE_COUNT
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

Piece_t * SpawnPiece(int type, int x, int y, int rotation);
void SavePlacedBlocks(Piece_t * piece, Block_t * head);
void RotatePiece(Piece_t * piece, Block_t * head);
void CurrentPieceGravity(Piece_t * piece);
void FreeMemory(Block_t * head, Piece_t * piece);
void ApplyCollisionsWalls(Piece_t * piece);
void ApplyCollisionsBlocks(Piece_t * piece, Block_t * head);
void GetMovementInput(Piece_t * piece, Block_t * head);
void LineExplosions(Block_t * head, bool * comboLines, int * score, int level);
bool DetectCombos(Block_t * head, bool * comboLines, int * comboCounter);
bool IsGameOver(Block_t * head);

#endif