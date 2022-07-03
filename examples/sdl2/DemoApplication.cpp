#include "DemoApplication.h"
#include "ParticleObject.h"
#include <SDL_image.h>
#include <exception>

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
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_sdlWindow = SDL_CreateWindow("SDL Test",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			screenWidth,
			screenHeight,
			SDL_WINDOW_SHOWN);
		if (_sdlWindow == nullptr)
		{
			printf("Window could not be initialized. SDL_Error: %s\n", SDL_GetError());
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
	if (!_isInitialised)
	{
		throw std::exception("DemoApplication is not initialised.");
	}

	ParticleObject snowParticles
		{ _sdlRenderer, "assets/bigStar.png", ParticleObject::ParticleStyle::SNOW, _screenWidth / 2,
		  _screenHeight / 2 };
	ParticleObject fireParticles
		{ _sdlRenderer, "assets/fire.png", ParticleObject::ParticleStyle::FIRE, _screenWidth / 2, _screenHeight / 2 };

	std::vector<ParticleObject*> particleObjects{};
	particleObjects.push_back(&fireParticles);
	particleObjects.push_back(&snowParticles);

	size_t index{ 0 };
	bool quitApplication{ false };
	SDL_Event sdlEvent{};

	while (!quitApplication)
	{
		while (SDL_PollEvent(&sdlEvent) != 0)
		{
			SDL_PumpEvents();

			switch (sdlEvent.type)
			{
			case SDL_QUIT:
			{
				quitApplication = true;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				if (sdlEvent.button.button == SDL_BUTTON_LEFT || SDL_BUTTON_RIGHT)
				{
					index++;
					if (index >= particleObjects.size())
					{
						index = 0;
					}
				}
			}
			default:
				break;
			}
		}
		SDL_RenderClear(_sdlRenderer);

		particleObjects[index]->draw();

		SDL_RenderPresent(_sdlRenderer);
		SDL_Delay(10);
	}
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
