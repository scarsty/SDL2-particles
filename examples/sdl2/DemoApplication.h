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
	SDL_Window* _sdlWindow { nullptr };
	SDL_Renderer* _sdlRenderer { nullptr };
	SDL_Surface* _sldScreenSurface { nullptr };
	bool _isInitialised { false };
};

#endif //CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_DEMOAPPLICATION_H_
