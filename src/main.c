//
// Created by Martin White on 08/01/2020.
//

#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

#include <clib/icon_protos.h>
#include <workbench/startup.h>

#define SDL_LogError( CAT, STR, ...) \
	printf(CAT STR "\n", ##__VA_ARGS__)

#define SDL_LOG_CATEGORY_APPLICATION    "[APPLICATION] "

void endTest(int);
void doDraw();

SDL_Surface *buffer = NULL;
SDL_Surface *screen = NULL;

SDL_PixelFormat format;
Uint32 startClock;
Uint32 deltaClock;
double fps = 0;

int width = 320;
int height = 240;

int main(int argc, char **argv) {
	struct WBStartup *wbStartup;
	struct DiskObject *diskObject;

	SDL_Event event;
	SDL_Rect **modes;
	unsigned int flags = SDL_DOUBLEBUF;

	int quit = 0;

	char *toolType;

	// Get our options
	if (argc != 0) {
		printf("You must start this program from the WB\n");
		return EXIT_FAILURE;
	}

	// Setup command line
	wbStartup = (struct WBStartup *)argv;

	// Process ToolTypes
	diskObject = GetDiskObject((unsigned char *)wbStartup->sm_ArgList[0].wa_Name);
	if (diskObject != NULL) {
		toolType = (char *) FindToolType(diskObject->do_ToolTypes, (unsigned char *) "HW_RENDER");
		flags = (toolType == NULL) ? SDL_SWSURFACE : SDL_HWSURFACE;

		toolType = (char *) FindToolType(diskObject->do_ToolTypes, (unsigned char *) "HIRES");
		if (toolType != NULL) {
			width = 640;
			height = 480;
		}

		toolType = (char *) FindToolType(diskObject->do_ToolTypes, (unsigned char *) "FULLSCREEN");
		if (toolType != NULL)
			flags |= SDL_FULLSCREEN;

		toolType = (char *)FindToolType(diskObject->do_ToolTypes, (unsigned char *)"BPP");
		format.BitsPerPixel = (toolType == NULL) ? 8 : strtod(toolType, NULL);;

		toolType = (char *) FindToolType(diskObject->do_ToolTypes, (unsigned char *) "CLOSE_WB");
		if (toolType != NULL)
			SDL_putenv("SDL_CLOSE_WB=1");

		toolType = (char *) FindToolType(diskObject->do_ToolTypes, (unsigned char *) "DISPLAY_MODE");
		if (toolType != NULL) {
			if (strcmp(toolType, "NTSC") == 0)
				SDL_putenv("SDL_DISPLAY_MODE=NTSC");
			else if (strcmp(toolType, "PAL") == 0)
				SDL_putenv("SDL_DISPLAY_MODE=PAL");
		}

		toolType = (char *) FindToolType(diskObject->do_ToolTypes, (unsigned char *) "RELATIVE_MOUSE");
		if (toolType != NULL)
			SDL_putenv("SDL_MOUSE_RELATIVE=1");

	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		exit(-1);
	}

	// TODO : Is this right when BPP > 8 ??
	format.BytesPerPixel = format.BitsPerPixel / 8;
	modes = SDL_ListModes(&format, flags);

	printf("Requested mode: %dx%d @ %dBpp %s\n",
	       width, height, format.BitsPerPixel,
	       (flags & SDL_FULLSCREEN) ? "FullScreen" : "Windowed");

	int foundRes = 0;
	if (modes == (SDL_Rect **)0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No sizes supported at this bit depth");
		endTest(-1);
	} else {
		printf("Listing modes...\n");
		for (int i = 0; modes[i]; ++i) {
			printf("Supported size w = %d h = %d\n", modes[i]->w, modes[i]->h);
			if (modes[i]->w == width && modes[i]->h == height)
				foundRes = 1;
		}

		if (modes == (SDL_Rect **)-1) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "All modes available");
			foundRes = 1;
		}
	}

	if (!foundRes) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Requested resolution (%dx%d) not available", width, height);
		endTest(-1);
	}

	if ((screen = SDL_SetVideoMode(width, height, format.BitsPerPixel, flags)) == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not set video mode: %s", SDL_GetError());
		endTest(-1);
	}

	//if ((buffer = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, format)))

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

uint32_t myRand(int max) {
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
	SDL_FillRect(screen, &rect, myRand(0xffffff + 1));

	/*
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = width;
	dest.h = height;
	SDL_BlitSurface(buffer, NULL, screen, &dest);
	*/

	SDL_Flip(screen);

	// update FPS counter
	deltaClock = SDL_GetTicks() - startClock;
	startClock = SDL_GetTicks();

	if(deltaClock > 0) {
		fps = (float)(1000.0f / (float)deltaClock);
	}

	printf("FPS: %6.2f\n", fps);
}

void endTest(int status) {
	if (buffer) {
		SDL_FreeSurface(buffer);
		buffer = NULL;
	}

	if (screen) {
		SDL_FreeSurface(screen);
		screen = NULL;
	}

	printf("SDL ENDED\n");

	SDL_Quit();

	exit(status);
}