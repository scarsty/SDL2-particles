#ifndef CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_DEMOAPPLICATION_H_
#define CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_DEMOAPPLICATION_H_
#include <SDL.h>

class DemoApplication
{
 public:
	bool initialise(int screenWidth, int screenHeight);
	void execute();
	void shutdown();

 private:
	SDL_Window* _sdlWindow;
	SDL_Renderer* _sdlRenderer;
	SDL_Surface* _sldScreenSurface;
};

#endif //CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_DEMOAPPLICATION_H_
