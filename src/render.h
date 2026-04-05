#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "piece.h"

void DrawMainMenu(Rectangle playButton);

void DrawInfo(int score, int level, int lineCounter);

void DrawGameOverMessage(int score);

void DrawTetrisGrid();

void DrawCurrentPiece(Piece_t * piece);

void DrawPlacedBlocks(Block_t * head);

#endif