#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <string>
using namespace std;
//=====================================
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//=====================================
const int TILE_SIZE = 16;

const int MAPWIDTH = 10;
const int MAPHEIGHT = 30;
const int GREY = 8;

//enum TILE_COLOR { TILE_NODRAW = -1, TILE_BLACK = 0, TILE_GREY, TILE_BLUE, TILE_RED, TILE_GREEN, TILE_YELLOW, TILE_WHITE, TILE_STEEL, TILE_PURPLE };
const int TILE_NODRAW = -1;
const int TILE_BLACK = 0;
const int TILE_GREY = 1;
const int TILE_BLUE = 2;
const int TILE_RED = 3;
const int TILE_GREEN = 4;
const int TILE_YELLOW = 5;
const int TILE_WHITE = 6;
const int TILE_STEEL = 7;
const int TILE_PURPLE = 8;
//========================================
/*class LTexture {
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};*/
//========================================\=\.
bool GameInit();						//|=| Initialize the game
//bool LoadMedia();						//|=| Load the block sprite sheet
void GameLoop();						//|=| Timing for block dropping
void GameDone();						//|=| Clean up
void DrawTile(int x, int y, int tile);	//|=| Drawing a tile, with coords, and tile type
void DrawMap();							//|=| Updating the map
void NewBlock();						//|=| Generating a new block
void RotateBlock();						//|=| Rotating the dropping block
void Move(int nx, int ny);				//|=| Move with directional arguments
int CollisionTest(int nx, int ny);		//|=| Test if blocks are in the way
void RemoveRow(int row);				//|=| Removal of a filled row
void NewGame();							//|=| Loser!
//========================================/=/===================================================\=\.
void Blit(SDL_Surface *Dest, int destX, int destY, SDL_Surface *Src);
void Blit(SDL_Surface *Dest, int DestX, int DestY, SDL_Surface *Src, int SrcX, int SrcY, int SrcW, int SrcH);		//This is used by the DrawTile() function
//========================================|=|===================================================/=/
SDL_Surface* gScreenSurface;			//|=| Mess with this later. It's probably outdated.
SDL_Window* gWindow = NULL;				//|=|
SDL_Renderer* gRenderer = NULL;			//|=|
SDL_Texture* texBlocks = NULL;			//|=|
//========================================|=| 
int Map[MAPWIDTH][MAPHEIGHT + 1];		//|=| The map is here.
//========================================|=| 
struct Piece {							//|=| 
	int size[4][4];						//|=| 
	int x;								//|=| This is defining a piece structure, for ease of coding.
	int y;								//|=|
};										//|=| 
//----------------------------------------|=| 
Piece sPrePiece;						//|=| This is the preview piece.
Piece sPiece;							//|=| The actual piece. the 's' denotes it's a structure.
//========================================|=|
double start_time;						//|=| This is used for timing.
bool GAMESTARTED = false;				//|=| Used by NewBlock().
//----------------------------------------|=|
SDL_Surface* surfBlocks;				//|=| Potentially old. Look into this.
//LTexture gBlockSheetTexture;			//|=| The sprite sheet
SDL_Rect gBlockClips[9];				//|=| 
bool GAMERUNNING = true;				//|=| Yeah.
//========================================\=\--------------------------------------------------------------------------\-\.
/*
LTexture::LTexture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free() {
	//Free texture if it exists
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}
*/
//========================================
void OnEvent() {
	SDL_Event Event;

	//=== Event Loop ===//
	//Grab any events in the queue
	while (SDL_PollEvent(&Event)) {
		switch (Event.type) {
		case SDL_QUIT:
			GAMERUNNING = false;
			break;
			
		case SDL_KEYDOWN:
			int Sym = Event.key.keysym.sym;
			switch (Sym) {
			case SDLK_ESCAPE:
				GAMERUNNING = false;
				break;
			case SDLK_DOWN:
				Move(0, 1);
				break;
			case SDLK_UP:
				RotateBlock();
				break;
			case SDLK_LEFT:
				Move(-1, 0);
				break;
			case SDLK_RIGHT:
				Move(1, 0);
				break;
			}
			break;
		}
	}
}
//===================================================================
int main(int argc, char* argv[]) {
	atexit(GameDone);
	bool success = true;
	
	//=== Start SDL Routines ===//
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	while (GAMERUNNING) {
		OnEvent();

		GameLoop();
		DrawMap();
	}
	return success;
} 
//=====================================
bool LoadMedia() {
	bool success = true;

	surfBlocks = SDL_LoadBMP("blocks.bmp");
	texBlocks = SDL_CreateTextureFromSurface(gRenderer, surfBlocks);
	//Load sprite sheet texture
	if (!texBlocks)
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{

		//Black!
		gBlockClips[0].x = 0;
		gBlockClips[0].y = 0;
		gBlockClips[0].w = 16;
		gBlockClips[0].h = 16;

		//Grey!
		gBlockClips[1].x = 16;
		gBlockClips[1].y = 0;
		gBlockClips[1].w = 16;
		gBlockClips[1].h = 16;

		//Blue!
		gBlockClips[2].x = 32;
		gBlockClips[2].y = 0;
		gBlockClips[2].w = 16;
		gBlockClips[2].h = 16;

		//Red!
		gBlockClips[3].x = 48;
		gBlockClips[3].y = 0;
		gBlockClips[3].w = 16;
		gBlockClips[3].h = 16;

		//Green!
		gBlockClips[4].x = 64;
		gBlockClips[4].y = 0;
		gBlockClips[4].w = 16;
		gBlockClips[4].h = 16;

		//Yellow!
		gBlockClips[5].x = 80;
		gBlockClips[5].y = 0;
		gBlockClips[5].w = 16;
		gBlockClips[5].h = 16;

		//Steel!
		gBlockClips[6].x = 96;
		gBlockClips[6].y = 0;
		gBlockClips[6].w = 16;
		gBlockClips[6].h = 16;

		//White!
		gBlockClips[7].x = 112;
		gBlockClips[7].y = 0;
		gBlockClips[7].w = 16;
		gBlockClips[7].h = 16;

		//Purple!
		gBlockClips[8].x = 128;
		gBlockClips[8].y = 0;
		gBlockClips[8].w = 16;
		gBlockClips[8].h = 16;
	}
	NewGame();
	return success;
}
//=====================================
void GameDone() {
	SDL_FreeSurface(gScreenSurface);
	SDL_FreeSurface(surfBlocks);
	SDL_Quit();
}
//=====================================
void GameLoop() {
	if ((SDL_GetTicks() - start_time) > 1000) {
		Move(0, 1);
		start_time = SDL_GetTicks();
	}
}
//=====================================
void NewGame() {
	start_time = SDL_GetTicks();
	GAMESTARTED = false;

	//Start the map
	for (int x = 0; x < MAPWIDTH; x++) {
		for (int y = 0; y < MAPHEIGHT; y++) {
			if (y == MAPHEIGHT)
				DrawTile(x, y, TILE_GREY);
			else
				DrawTile(x, y, TILE_BLACK);
		}
	}
	NewBlock();
	DrawMap();
}
//=====================================
void DrawTile(int x, int y, int tile) {
	Blit(gScreenSurface, x*TILE_SIZE, y*TILE_SIZE, surfBlocks, tile*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
}
//=====================================
void DrawMap() {
	int xmy, ymx;

	// Place the toolbar
	for (xmy = MAPWIDTH; xmy < MAPWIDTH + GREY; xmy++)
		for (ymx = 0; ymx < MAPHEIGHT; ymx++)
			DrawTile(xmy, ymx, TILE_GREY);

	// Draw a preview block
	for (xmy = 0; xmy < 4; xmy++)
		for (ymx = 0; ymx < 4; ymx++)
			if (sPrePiece.size[xmy][ymx] != TILE_NODRAW)
				DrawTile(sPrePiece.x + xmy, sPrePiece.y + ymx, sPrePiece.size[xmy][ymx]);

	// Draw the map
	// Loop through positions
	for (xmy = 0; xmy < MAPWIDTH; xmy++)
		for (ymx = 0; ymx < MAPHEIGHT; ymx++)
			DrawTile(xmy, ymx, Map[xmy][ymx]);

	//Draw the moving block
	for (xmy = 0; xmy < 4; xmy++)
		for (ymx = 0; ymx < 4; ymx++)
			if (sPiece.size[xmy][ymx] != TILE_NODRAW)
				DrawTile(sPiece.x + xmy, sPiece.y + ymx, sPiece.size[xmy][ymx]);

	//Do render stuff here, whatever it is
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, texBlocks, NULL, NULL);
	SDL_RenderPresent(gRenderer);
}
//=====================================
void NewBlock() {
	int newblock;
	int i, j;
	// Preview of which blocks are which.
	//==0===1=====2======3======4=====5====6===\-\.
	//--X--------------------------------------|-|
	//--X--X-X----X----X-X------X-X--X-X--X-X--|-|
	//--X--X-X--X-X-X----X-X--X-X------X--X----|-|
	//--X------------------------------X--X----|-|
	//=========================================/-/.

	srand(SDL_GetTicks());

	//Initialize the piece to all blank
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			sPiece.size[i][j] = TILE_NODRAW;

	sPiece.x = MAPWIDTH / 2 - 2;
	sPiece.y = -1;

	if (!GAMESTARTED) {
		GAMESTARTED = true;

		newblock = rand() % 7;

		switch (newblock)
		{
		case 0: //Tower!
		{
			sPiece.size[1][0] = TILE_RED;
			sPiece.size[1][1] = TILE_RED;
			sPiece.size[1][2] = TILE_RED;
			sPiece.size[1][3] = TILE_RED;
			sPiece.y = 0;
		}break;
		case 1: //Box!
		{
			sPiece.size[1][1] = TILE_BLUE;
			sPiece.size[1][2] = TILE_BLUE;
			sPiece.size[2][1] = TILE_BLUE;
			sPiece.size[2][2] = TILE_BLUE;
		}break;
		case 2: //Pyramid!
		{
			sPiece.size[1][1] = TILE_STEEL;
			sPiece.size[0][2] = TILE_STEEL;
			sPiece.size[1][2] = TILE_STEEL;
			sPiece.size[2][2] = TILE_STEEL;
		}break;
		case 3://Left Leaner
		{
			sPiece.size[0][1] = TILE_YELLOW;
			sPiece.size[1][1] = TILE_YELLOW;
			sPiece.size[1][2] = TILE_YELLOW;
			sPiece.size[2][2] = TILE_YELLOW;
		}break;
		case 4://Right Leaner
		{
			sPiece.size[2][1] = TILE_GREEN;
			sPiece.size[1][1] = TILE_GREEN;
			sPiece.size[1][2] = TILE_GREEN;
			sPiece.size[0][2] = TILE_GREEN;
		}break;
		case 5://Left Knight
		{
			sPiece.size[1][1] = TILE_WHITE;
			sPiece.size[2][1] = TILE_WHITE;
			sPiece.size[2][2] = TILE_WHITE;
			sPiece.size[2][3] = TILE_WHITE;
		}break;
		case 6://Right Knight
		{
			sPiece.size[2][1] = TILE_PURPLE;
			sPiece.size[1][1] = TILE_PURPLE;
			sPiece.size[1][2] = TILE_PURPLE;
			sPiece.size[1][3] = TILE_PURPLE;
		}break;
		}
	}
	else {
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				sPiece.size[i][j] = sPrePiece.size[i][j];
	}

	newblock = rand() % 7;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			sPrePiece.size[i][j] = TILE_NODRAW;

	sPrePiece.x = MAPWIDTH + GREY / 4;
	sPrePiece.y = GREY / 4;

	switch (newblock)
	{
	case 0: //Tower!
	{
		sPrePiece.size[1][0] = TILE_RED;
		sPrePiece.size[1][1] = TILE_RED;
		sPrePiece.size[1][2] = TILE_RED;
		sPrePiece.size[1][3] = TILE_RED;
	}break;
	case 1: //Box!
	{
		sPrePiece.size[1][1] = TILE_BLUE;
		sPrePiece.size[1][2] = TILE_BLUE;
		sPrePiece.size[2][1] = TILE_BLUE;
		sPrePiece.size[2][2] = TILE_BLUE;
	}break;
	case 2: //Pyramid!
	{
		sPrePiece.size[1][1] = TILE_STEEL;
		sPrePiece.size[0][2] = TILE_STEEL;
		sPrePiece.size[1][2] = TILE_STEEL;
		sPrePiece.size[2][2] = TILE_STEEL;
	}break;
	case 3://Left Leaner
	{
		sPrePiece.size[0][1] = TILE_YELLOW;
		sPrePiece.size[1][1] = TILE_YELLOW;
		sPrePiece.size[1][2] = TILE_YELLOW;
		sPrePiece.size[2][2] = TILE_YELLOW;
	}break;
	case 4://Right Leaner
	{
		sPrePiece.size[2][1] = TILE_GREEN;
		sPrePiece.size[1][1] = TILE_GREEN;
		sPrePiece.size[1][2] = TILE_GREEN;
		sPrePiece.size[0][2] = TILE_GREEN;
	}break;
	case 5://Left Knight
	{
		sPrePiece.size[1][1] = TILE_WHITE;
		sPrePiece.size[2][1] = TILE_WHITE;
		sPrePiece.size[2][2] = TILE_WHITE;
		sPrePiece.size[2][3] = TILE_WHITE;
	}break;
	case 6://Right Knight
	{
		sPrePiece.size[2][1] = TILE_PURPLE;
		sPrePiece.size[1][1] = TILE_PURPLE;
		sPrePiece.size[1][2] = TILE_PURPLE;
		sPrePiece.size[1][3] = TILE_PURPLE;
	}break;
	}

	DrawMap();
}
//=====================================
void RotateBlock()
{
	int i, j, temp[4][4];

	// Copy and rotate the piece to the temp array
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			temp[3 - j][i] = sPiece.size[i][j];

	// Check collision of the temp array with map borders
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (temp[i][j] != TILE_NODRAW)
				if (sPiece.x + i < 0 || sPiece.x + i > MAPWIDTH - 1 ||
					sPiece.y + j < 0 || sPiece.y + j > MAPHEIGHT - 1)
					return;

	// Check collision of the temp array with other placed blocks
	for (int x = 0; x < MAPWIDTH; x++)
		for (int y = 0; y < MAPHEIGHT; y++)
			if (x >= sPiece.x && x < sPiece.x + 4)
				if (y >= sPiece.y && y < sPiece.y + 4)
					if (Map[x][y] != TILE_BLACK)
						if (temp[x - sPiece.x][y - sPiece.y] != TILE_NODRAW)
							return;

	// End collision check

	// Success! Copy the rotated temp array to the original piece
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			sPiece.size[i][j] = temp[i][j];

	DrawMap();
}
//=====================================
void Move(int x, int y)
{
	if (CollisionTest(x, y)) {
		if (y == 1) {
			if (sPiece.y < 1) {
				NewGame();			// Loser!
			}
			else {
				bool killBlock = false;
				int i, j;

				// Time for a new block! Add it to the list!
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						if (sPiece.size[i][j] != TILE_NODRAW)
							Map[sPiece.x + i][sPiece.y + j] = sPiece.size[i][j];

				// Check for cleared rows.
				for (j = 0; j < MAPHEIGHT; j++) {
					bool filled = true;
					for (i = 0; i < MAPWIDTH; i++)
						if (Map[i][j] == TILE_BLACK)
							filled = false;
					if (filled) {
						RemoveRow(j);
						killBlock = true;
					}
				}

				if (killBlock) {
					for (i = 0; i < 4; i++)
						for (j = 0; j < 4; j++)
							sPiece.size[i][j] = TILE_NODRAW;
				}
				NewBlock();
			}
		}
	}
	else {
		sPiece.x += x;
		sPiece.y += y;
	}

	DrawMap();
}
//=====================================
int CollisionTest(int nx, int ny)
{
	int newx = sPiece.x + nx;
	int newy = sPiece.y + ny;

	int i, j, x, y;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (sPiece.size[i][j] != TILE_NODRAW)
				if (newx + i < 0 || newx + i > MAPWIDTH - 1 ||
					newy + j < 0 || newy + j > MAPHEIGHT - 1)
					return 1;

	for (x = 0; x < MAPWIDTH; x++)
		for (y = 0; y < MAPHEIGHT; y++)
			if (x >= newx && x < newx + 4)
				if (y >= newy && y < newy + 4)
					if (Map[x][y] != TILE_BLACK)
						if (sPiece.size[x - newx][y - newy] != TILE_NODRAW)
							return 1;

	return 0;
}
//==================================================================
void RemoveRow(int row)
{
	int x, y;
	int counter = 0;

	for (x = 0; x < MAPWIDTH; x++)
		for (y = row; y > 0; y--)
			Map[x][y] = Map[x][y - 1];
}
//==================================================================
void Blit(SDL_Surface *Dest, int DestX, int DestY, SDL_Surface *Src)
{
	// Create a rectangle, and store the coords in it. SDL likes that.
	SDL_Rect DestR;
	DestR.x = DestX; DestR.y = DestY;

	// Draw to that Dest thing
	SDL_BlitSurface(Src, NULL, Dest, &DestR);
}
//==================================================================
void Blit(SDL_Surface *Dest, int DestX, int DestY, SDL_Surface *Src, int SrcX, int SrcY, int SrcW, int SrcH)
{
	// Create two rectangles. The first is where we blit to, and the other is for clipping the 'Src' so we only draw what we want.

	SDL_Rect DestR;  SDL_Rect SrcR;
	DestR.x = DestX; DestR.y = DestY;
	SrcR.x = SrcX;   SrcR.y = SrcY;
	SrcR.w = SrcW;   SrcR.h = SrcH;

	// Draw to the Dest
	SDL_BlitSurface(Src, &SrcR, Dest, &DestR);

}
