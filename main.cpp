/*
	GOL - An implementation of Conway's Game of Life
	Author:	Angus Wilson (angus.wilson13@education.nsw.gov.au)
	Date:	March 2021
	
	This program uses the raylib library for graphics and input.
	Information on compiling software with raylib can be found at the link:
	https://github.com/raysan5/raylib/wiki
	
	Todo:
	* Help screen showing all options
	* Config file parsing and loading (width, height, tileSize)
	* File loading and saving in a binary format
	* Music generation. Each 5 cells on one axis corresponds to a note on the
	  pentatonic scale, the other axis possibly controlling a filter envelope.
	  Not strictly necessary but would be cool.
*/
#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Width and height of the map in tiles
const int width = 80;
const int height = 80;

// Size of tiles on screen in pixels
const int tileSize = 10;

const bool startupPattern = true;

// We store the map twice in memory when working with it so we can change the
// state of the map while still getting correct output based on the prior map.
bool map[width][height];
bool lastMap[width][height];

// Variables used to control the speed of the map updating. 
// When paused is set to true the map does not update.
float timer;
float timerLength = 0.04;
bool paused = true;

// Iterates through the map setting every value to 0 a.k.a false.
// Called when the user presses c and the simulation is paused.
void clearMap(){
	for(int i=0;i<=width;i++){
		for(int j=0;j<=height;j++){	
			lastMap[i][j] = 0;
			map[i][j] = 0;
		}
	}
}

// Updates the map with reference to the previous iteration
// This may or may not require a rewrite for clarity.
void updateMap(int rows, int cols, bool map[width][height]){
	// Sets the values of lastMap to those in map.
	// This used to be another function called cpymap()
	for(int i=0;i<=width;i++){
		for(int j=0;j<=height;j++){
			lastMap[i][j] = map[i][j];  
		}
	}
	// Iterates over each cell.
	for(int i=0;i<=rows;i++){
		for(int j=0;j<=cols;j++){
			// neighbours is -1 because the cell will count itself within it's own 3x3 area.
			// Therefore neighbours will always be at least 0 for live cells.
			// This is probably more efficient than an if condition in the 3x3 loop
			int neighbours = -  0 - lastMap[i][j];

			// Iterates over the 9 squares within the cell's 3x3 area and adds to neighbours
			// if the cell is set to true. Essentially just counts neighbours in the 3x3 area.
			for (int a=0;a<3;a++){
				for(int b=0;b<3;b++){
					if(lastMap[i-1+a][j-1+b]){
						neighbours++;
					}
				}
			}
			// Checks if cell is alive
			if (map[i][j]){
				// Kills all living cells that ought to die in the next generation by the rules
				if (!(neighbours == 2 || neighbours == 3)){
					map[i][j] = 0;
				}
			}
			// Creates any dead cells that should be alive in the next generation.
			// This is set to 2 instead of 3 by the rules of the game because all cells start
			// counting neighbours at -1, therefore 3 = 2 for dead cells.
			// this could be aleviated using an if statement but that would be a bit less efficient
			else if(neighbours == 3){
				map[i][j] = 1;
			}
		}
	}
}

// Draws the tilemap onto the screen at the specified coordinates
void drawTiles(int x,int y){
	// Loop over the map with a different method because i didnt feel like passing in the tiles
	// Maybe not the best way to do it. Refactor so its done in a functional way? (no side effects)
	for (size_t i = 0; i < sizeof(map)  / sizeof(*map); ++i)
		for (size_t j = 0; j < sizeof(*map) / sizeof(**map);  ++j) {

		// Draws a square at the correct position if the cell is alive
		switch(map[i][j]) {
			case 1:
				DrawRectangle(j * tileSize + x, i * tileSize + y, tileSize, tileSize, BLACK);
				break;
			default:
				break;
		}
	}
}

void drawGrid(){
	// Draws horizontal lines based on width, height, and tileSize
	for (int i = 1; i <= width; ++i){
		DrawLineEx(Vector2{i * tileSize,0},Vector2{i * tileSize,height * tileSize},1,LIGHTGRAY);
	}
	// Does the same for vertical lines
	for (int i = 1; i <= height; ++i){
		DrawLineEx(Vector2{0,i * tileSize},Vector2{width * tileSize,i * tileSize},1,LIGHTGRAY);
	}
}


// Called to update input and the map
// update is seperated from draw because this is how a reasonable game loop works.
void update(){
	// dt is the time since the last frame rendered in seconds. e.g. at 60fps it would be  0.016
	float dt = GetFrameTime();
	if (!paused) timer += dt; // Iterate the timer if the program is not paused

	// update the map if the timer has reached it's length and the program is not paused
	if (timer >= timerLength && !paused){
		updateMap(width,height,map);
	}
	// If the timer is in the reasonable boundaries of speed increase the speed with the
	// mouse wheel movement * 0.005
	if (timerLength >= 0 && timerLength <= 0.100001){
		timerLength += GetMouseWheelMove() * -0.005f;
	}
	// Reset the timer if it escapes reasonable bounds
	if (timerLength < 0) timerLength = 0;
	if (timerLength > 0.1) timerLength = 0.1;

	if (IsKeyPressed(KEY_SPACE)) paused = !paused; // Pause if space is pressed.
	
	if (paused){
		if (IsKeyPressed(KEY_S)) updateMap(width,height,map); // S to step through iterations
		
		// Flip the tile at the mouse position when clicked
		if (IsMouseButtonPressed(0)){
			int mouseX = GetMouseX();
			int mouseY = GetMouseY();
			int mouseYtile = floor(mouseX / tileSize);
			int mouseXtile = floor(mouseY / tileSize);
			map[mouseXtile][mouseYtile] = !map[mouseXtile][mouseYtile];
		}
		if (IsKeyPressed(KEY_C)) clearMap(); // Clear the map with C
	}
}

// Draws the map onto the screen
void draw(){
BeginDrawing();
	ClearBackground(WHITE); // Clear the background to white.
	drawTiles(0,0);
	if (paused) drawGrid(); // Draw the grid

	// Draws UI to display the speed. Rectangle is drawn for increased legibility.
	DrawRectangle(0,0,180,30,WHITE);
	DrawText(TextFormat("speed: %f", 100-1000*timerLength),5,5,20,BLUE);
EndDrawing();
}


// Entrypoint
int main(void){
	// calculates the correct size of the screen in pixels
	const int screenwidth = width * tileSize;
	const int screenheight = height * tileSize;

	// Initializes the window
	InitWindow(screenwidth, screenheight, "Game Of Life");

	// Sets fps to 60
	SetTargetFPS(60);

	if (startupPattern){
		/*	
			There is a glider pattern that shows up on startup by default
			
			+---+
			| # |  It looks like this.
			|  #|
			|###|
			+---+
		
			These are the positions on the map for the glider
		*/
		map[11][12] = 1;
		map[12][13] = 1;
		map[13][11] = 1;
		map[13][12] = 1;
		map[13][13] = 1;
	}
	// Main Loop
	while (!WindowShouldClose()) // WindowShouldClose() returns true when esc is pressed
	{
		update();
		draw();
	}
	CloseWindow();

	return 0;
}