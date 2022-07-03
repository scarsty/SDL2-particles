
#ifndef CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_PARTICLEOBJECT_H_
#define CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_PARTICLEOBJECT_H_
#include <ParticleSystem.h>
#include <exception>
#include <SDL.h>
#include <SDL_image.h>

class ParticleObject
{
 public:
	enum ParticleStyle
	{
		NONE,
		SNOW,
		FIRE,
		FIREWORK,
		EXPLOSION,
		SMOKE,
		GALAXY,
		RAIN,
		METEOR,
		FLOWER,
	};

	ParticleObject(SDL_Renderer* renderer, const char* texturePath, ParticleStyle style, int positionX, int positionY);

	~ParticleObject();
	ParticleObject(const ParticleObject& other) = delete;
	ParticleObject& operator=(const ParticleObject& other) = delete;
	ParticleObject(ParticleObject&& other) = delete;
	ParticleObject& operator=(ParticleObject&& other) = delete;

	void setStyle(ParticleStyle style);
	void draw();
	void reset();

 private:
	ParticleSystem _particleSystem{};
	SDL_Texture* _particleTexture;
	SDL_Renderer* _particleRenderer;
	ParticleStyle _style;
};

#endif //CPP_PARTICLE_SYSTEM_EXAMPLES_SDL2_PARTICLEOBJECT_H_
