#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "raylib.h"
#include "config.h"
#include "piece.h"
#include "game.h"
#include "render.h"
#include "utils.h"

int main(void) {
    
    //INITIALIZATION
    Game_t g;
    InitializeGame(&g);
    
    //GAMELOOP
    while (!WindowShouldClose()) {
        
        GameInput(&g);
        GameUpdate(&g);
        GameRender(&g);
    }
    //DE-INITIALIZATION
    KillGame(&g);
    return 0;
}