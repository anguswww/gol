#include "raylib.h"
#include <cstddef>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h> 

#define WIDTH   80
#define HEIGHT  80
#define TILESIZE 8

bool map[WIDTH][HEIGHT];
bool lastMap[WIDTH][HEIGHT];
float timer;
float timerLength = 0.04;
bool paused = true;

void updateMap(int rows, int cols, bool map[WIDTH][HEIGHT]){
    for(int i=0;i<=WIDTH;i++){
        for(int j=0;j<=HEIGHT;j++){
            lastMap[i][j] = map[i][j];  
        }
    }
    for(int i=0;i<=rows;i++){
        for(int j=0;j<=cols;j++){
        int neighbours = -1;

        for (int a=0;a<3;a++){
            for(int b=0;b<3;b++){
                if(lastMap[i-1+a][j-1+b]){
                    neighbours++;
                }
            }
        }
        if (map[i][j]){
            switch(neighbours){

                case 2:
                case 3:
                    break;
                default:
                    map[i][j] = 0;
            } 
        }else{
            switch(neighbours){
                case 2:
                    map[i][j] = 1;
                    break;
            }
        }

        }
    }
}


void drawTiles(int x,int y){
    for (size_t i = 0; i < sizeof(map)  / sizeof(*map); ++i)
    for (size_t j = 0; j < sizeof(*map) / sizeof(**map);  ++j) {
      switch(map[i][j]) {
        case 1:
            DrawRectangle(j * TILESIZE + x, i * TILESIZE + y, TILESIZE, TILESIZE, BLACK);
            break;
        default:
            break;
      }
    }
}
void update(){
    float dt = GetFrameTime();
    if (!paused) timer += dt;
    if (timer >= timerLength){
        updateMap(WIDTH,HEIGHT,map);
    }
    timerLength += GetMouseWheelMove() * -0.005f;
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    if (paused){
        if (IsKeyPressed(KEY_S)) updateMap(WIDTH,HEIGHT,map);
    }
}

void draw(){
    BeginDrawing();

    ClearBackground(RAYWHITE);
    drawTiles(0,0);
    EndDrawing();
}

int main(void){
    const int screenWidth = WIDTH * TILESIZE;
    const int screenHeight = HEIGHT * TILESIZE;

    InitWindow(screenWidth, screenHeight, "Game Of Life");

    SetTargetFPS(60);

    map[1+10][2+10] = 1;
    map[2+10][3+10] = 1;
    map[3+10][1+10] = 1;
    map[3+10][2+10] = 1;
    map[3+10][3+10] = 1;
    while (!WindowShouldClose())
    {
        update();
        draw();
    }
    CloseWindow();


    return 0;
}