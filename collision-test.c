#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void applyCollisions(Rectangle * player, Rectangle * object) {

    if (CheckCollisionRecs(*player, *object)) {

        Rectangle overlap = GetCollisionRec(*player, *object);
        
        if (overlap.width < overlap.height) {
            if (player->x < object->x) {player->x -= overlap.width;}
            else {player->x += overlap.width;}
        }
        
        else {
            if (player->y < object->y) {player->y -= overlap.height;}
            else {player->y += overlap.height;}
        }


    }
}

int main(void) {

    Rectangle player;
    player.height = 100;
    player.width = 100;
    player.x = 300;
    player.y = 1080 / 2; 

    Rectangle object;
    object.height = 100;
    object.width = 100;
    object.x = 1200;
    object.y = 1080 / 2;

    InitWindow(1920, 1080, "Window");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        //UPDATE
        if (IsKeyDown(KEY_A)) {
            player.x -= 5.0f;
        }

        if (IsKeyDown(KEY_S)) {
            player.y += 5.0f;
        }

        if (IsKeyDown(KEY_D)) {
            player.x += 5.0f;
        }

        if (IsKeyDown(KEY_W)) {
            player.y -= 5.0f;
        }

        applyCollisions(&player, &object);

        //DRAW
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleRec(player, BLUE);
        DrawRectangleRec(object, RED);
        EndDrawing();
    }
    

}