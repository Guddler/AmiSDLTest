//
// Created by Martin White on 08/01/2020.
//

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#define SDL_LogError( CAT, STR, ...) \
	printf(CAT STR "\n", ##__VA_ARGS__)

#define SDL_LOG_CATEGORY_APPLICATION    "[APPLICATION] "

void endTest(int);
void doDraw();

SDL_Surface *frameBuffer;
SDL_PixelFormat format;
Uint32 startClock;
Uint32 deltaClock;
double fps = 0;

int width = 320;
int height = 240;

int main(int argc, char **argv) {
	SDL_Event event;
	SDL_Rect **sizes;
	int flags = SDL_SWSURFACE | SDL_FULLSCREEN;

	int quit = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		exit(-1);
	}

	format.BitsPerPixel = 8;
	format.BytesPerPixel = 1;
	sizes = SDL_ListModes(&format, flags);

	printf("Requested mode: %dx%d @ %dBpp %s\n",
	       width, height, format.BitsPerPixel,
	       (flags & SDL_FULLSCREEN) ? "FullScreen" : "Windowed");

	int foundRes = 0;
	if (sizes == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No sizes supported at this bit depth");
		endTest(-1);
	} else {
		for (int i = 0; sizes[i]; i++) {
			printf("Supported size w = %d h = %d\n", sizes[i]->w, sizes[i]->h);
			if (sizes[i]->w == width && sizes[i]->h == height)
				foundRes = 1;
		}
	}

	if (!foundRes) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Requested resolution (%dx%d) not available", width, height);
		endTest(-1);
	}

	if ((frameBuffer = SDL_SetVideoMode(width, height, format.BitsPerPixel, flags)) == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not set video mode: %s", SDL_GetError());
		endTest(-1);
	}

	SDL_WM_SetCaption("Test", "Test");

	startClock = SDL_GetTicks();

	while (!quit) {
		// Main loop
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				// Window close icon
				case SDL_QUIT :
					quit = 1;
					break;
				// Escape key
				case SDL_KEYDOWN :
					if (event.key.keysym.sym == SDLK_ESCAPE)
						quit = 1;
					break;
			}
		}

		doDraw();
	}

	endTest(0);
}

int myRand(int max) {
	srand(SDL_GetTicks());
	return (rand() % (max + 1));
}

void doDraw() {

	SDL_Rect rect;
	rect.x = myRand(width);
	rect.y = myRand(height);
	rect.w = myRand(width / 4);
	rect.h = myRand(height / 4);

	// I know the colours are wrong here - just testing
	SDL_Color col;
	col.r = myRand(255);
	col.g = myRand(255);
	col.b = myRand(255);

	SDL_FillRect(frameBuffer, &rect, SDL_MapRGB(&format, col.r, col.g, col.b));

	SDL_Flip(frameBuffer);

	// update FPS counter
	deltaClock = SDL_GetTicks() - startClock;
	startClock = SDL_GetTicks();

	if(deltaClock > 0) {
		fps = (float)(1000.0f / (float)deltaClock);
	}

	printf("FPS: %6.2f\n", fps);
}

void endTest(int status) {
	if (frameBuffer) {
		SDL_FreeSurface(frameBuffer);
		frameBuffer = NULL;
	}

	printf("SDL ENDED\n");

	SDL_Quit();

	exit(status);
}