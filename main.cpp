#include "raylib.h"
#include <cstddef>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define WIDTH   80
#define HEIGHT  80
#define TILESIZE 10

bool map[WIDTH][HEIGHT];
bool lastMap[WIDTH][HEIGHT];
float timer;
float timerLength = 0.04;
bool paused = true;

void clearMap(){
    for(int i=0;i<=WIDTH;i++){
        for(int j=0;j<=HEIGHT;j++){
            lastMap[i][j] = 0;
            map[i][j] = 0;  
        }
    }
}

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

void drawGrid(){
    for (int i = 1; i <= WIDTH; ++i){
        DrawLineEx(Vector2{i * TILESIZE,0},Vector2{i * TILESIZE,HEIGHT * TILESIZE},1,LIGHTGRAY);
    }
    for (int i = 1; i <= HEIGHT; ++i){
        DrawLineEx(Vector2{0,i * TILESIZE},Vector2{WIDTH * TILESIZE,i * TILESIZE},1,LIGHTGRAY);
    }
}

void update(){
    float dt = GetFrameTime();
    if (!paused) timer += dt;
    if (timer >= timerLength && !paused){
        updateMap(WIDTH,HEIGHT,map);
    }
    if (timerLength >= 0 && timerLength <= 0.100001){
        timerLength += GetMouseWheelMove() * -0.005f;
    }
    if (timerLength < 0) timerLength = 0;
    if (timerLength > 0.1) timerLength = 0.1;
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    if (paused){
        if (IsKeyPressed(KEY_S)) updateMap(WIDTH,HEIGHT,map);
        if (IsMouseButtonPressed(0)){
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            int mouseYtile = floor(mouseX / TILESIZE);
            int mouseXtile = floor(mouseY / TILESIZE);
            map[mouseXtile][mouseYtile] = !map[mouseXtile][mouseYtile];
        }
        if (IsKeyPressed(KEY_C)) clearMap();
    }
}

void draw(){
    BeginDrawing();

    ClearBackground(RAYWHITE);
    drawTiles(0,0);
    if (paused) drawGrid();
    DrawText(TextFormat("speed: %f", 100-1000*timerLength),5,5,20,BLUE);
    EndDrawing();
}

int main(void){
    const int screenWidth = WIDTH * TILESIZE;
    const int screenHeight = HEIGHT * TILESIZE;

    InitWindow(screenWidth, screenHeight, "Game Of Life");

    SetTargetFPS(60);

    map[11][12] = 1;
    map[12][13] = 1;
    map[13][11] = 1;
    map[13][12] = 1;
    map[13][13] = 1;
    while (!WindowShouldClose())
    {
        update();
        draw();
    }
    CloseWindow();

    return 0;
}