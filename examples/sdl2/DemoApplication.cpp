#include "DemoApplication.h"
#include <SDL_image.h>

bool DemoApplication::initialise(int screenWidth, int screenHeight)
{
	if (_isInitialised)
	{
		return true;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not initialize SDL. SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		_sdlWindow = SDL_CreateWindow("SDL Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screenWidth,
			screenHeight,
			SDL_WINDOW_SHOWN);
		if (_sdlWindow == nullptr)
		{
			printf("Window could not be initialized. SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			_sdlRenderer = SDL_CreateRenderer(_sdlWindow, -1, SDL_RENDERER_ACCELERATED);

			if (!_sdlRenderer)
			{
				printf("Renderer could not be created, SDL_Error: %s\n", SDL_GetError());
				return false;
			}

			//Initialize PNG loading
			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
			{
				printf("SDL_image could not be initialized. SDL_image Error: %s\n", IMG_GetError());
				return false;
			}

			//Get window surface
			_sldScreenSurface = SDL_GetWindowSurface(_sdlWindow);

			if (!_sldScreenSurface)
			{
				printf("Surface could not be created, SDL_Error: %s\n", SDL_GetError());
				return false;
			}

		}
	}

	_isInitialised = true;
	return true;
}

void DemoApplication::execute()
{
	//TODO afmaken

}

void DemoApplication::shutdown()
{
	if (_isInitialised)
	{
		if (_sldScreenSurface)
		{
			SDL_FreeSurface(_sldScreenSurface);
			_sldScreenSurface = nullptr;
		}

		if (_sdlRenderer)
		{
			SDL_DestroyRenderer(_sdlRenderer);
			_sdlRenderer = nullptr;
		}

		if (_sdlWindow)
		{
			SDL_DestroyWindow(_sdlWindow);
			_sdlWindow = nullptr;
		}

		SDL_Quit();
		_isInitialised = false;
	}
}
