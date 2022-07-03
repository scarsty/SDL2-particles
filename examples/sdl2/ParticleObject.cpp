#include "ParticleObject.h"

ParticleObject::ParticleObject(SDL_Renderer* renderer,
	const char* texturePath,
	ParticleObject::ParticleStyle style,
	int positionX,
	int positionY) : _particleRenderer{ renderer }, _style{ style }
{
	setStyle(style);
	_particleSystem.setPosition(static_cast<float>(positionX), static_cast<float>(positionY));

	SDL_Texture* particleTexture = IMG_LoadTexture(_particleRenderer, texturePath);
	if (particleTexture == nullptr)
	{
		throw std::exception("Could not load the particle texture.");
	}
	else
	{
		_particleTexture = particleTexture;
	}
}

ParticleObject::~ParticleObject()
{
	if (!_particleTexture)
	{
		SDL_DestroyTexture(_particleTexture);
		_particleTexture = nullptr;
	}
}

void ParticleObject::draw()
{
	for (int i = 0; i < _particleSystem.getParticleCount(); i++)
	{
		auto particleData = _particleSystem.getParticleData(i);
		if (particleData._size <= 0 || particleData._colorA <= 0)
		{
			continue;
		}
		SDL_Rect r =
			{ int(particleData._posx + particleData._startPosX - particleData._size / 2), int(particleData._posy + particleData._startPosY - particleData._size / 2), int(particleData._size),
			  int(particleData._size) };
		SDL_Color
			c = { Uint8(particleData._colorR * 255), Uint8(particleData._colorG * 255), Uint8(particleData._colorB * 255), Uint8(particleData._colorA * 255) };
		SDL_SetTextureColorMod(_particleTexture, c.r, c.g, c.b);
		SDL_SetTextureAlphaMod(_particleTexture, c.a);
		SDL_SetTextureBlendMode(_particleTexture, SDL_BLENDMODE_BLEND);
		SDL_RenderCopyEx(_particleRenderer, _particleTexture, nullptr, &r, particleData._rotation, nullptr, SDL_FLIP_NONE);
	}

	_particleSystem.update();
}

void ParticleObject::setStyle(ParticleStyle style)
{
	_style = style;
	_particleSystem.resetSystem();

	switch (style)
	{
	case NONE:
	{
		_particleSystem.stopSystem();
		return;
	}
	case SNOW:
	{
		_particleSystem.initWithTotalParticles(700);

		// duration
		_particleSystem.setDuration(ParticleSystem::DURATION_INFINITY);

		// set gravity mode.
		_particleSystem.setEmitterMode(ParticleSystem::Mode::GRAVITY);

		// Gravity Mode: gravity
		_particleSystem.setGravity(Vec2(0, 1));

		// Gravity Mode: speed of particles
		_particleSystem.setSpeed(-5);
		_particleSystem.setSpeedVar(1);

		// Gravity Mode: radial
		_particleSystem.setRadialAccel(0);
		_particleSystem.setRadialAccelVar(1);

		// Gravity mode: tangential
		_particleSystem.setTangentialAccel(0);
		_particleSystem.setTangentialAccelVar(1);

		// angle
		_particleSystem.setAngle(-90);
		_particleSystem.setAngleVar(5);

		// life of particles
		_particleSystem.setLife(15);

		// size, in pixels
		_particleSystem.setStartSize(20.0f);
		_particleSystem.setStartSizeVar(5.0f);
		_particleSystem.setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);

		// emits per second
		_particleSystem.setEmissionRate(10);

		// color of particles
		_particleSystem.setStartColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		_particleSystem.setStartColorVar({ 0.0f, 0.0f, 0.0f, 0.0f });
		_particleSystem.setEndColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		_particleSystem.setEndColorVar({ 0.0f, 0.0f, 0.0f, 0.0f });

		_particleSystem.setPosVar({ 1.0f * 300, 0.0f });
		break;
	}
	case FIRE:
	{
		int totalParticles{ 250 };
		_particleSystem.initWithTotalParticles(totalParticles);

		// duration
		_particleSystem.setDuration(ParticleSystem::DURATION_INFINITY);

		// Gravity Mode
		_particleSystem.setEmitterMode(ParticleSystem::Mode::GRAVITY);

		// Gravity Mode: gravity
		_particleSystem.setGravity(Vec2(0, 0));

		// Gravity Mode: radial acceleration
		_particleSystem.setRadialAccel(0);
		_particleSystem.setRadialAccelVar(0);

		// Gravity Mode: speed of particles
		_particleSystem.setSpeed(-60);
		_particleSystem.setSpeedVar(20);

		// starting angle
		_particleSystem.setAngle(90);
		_particleSystem.setAngleVar(10);

		// life of particles
		float life{ 3 };
		_particleSystem.setLife(life);
		_particleSystem.setLifeVar(0.25f);

		// size, in pixels
		_particleSystem.setStartSize(54.0f);
		_particleSystem.setStartSizeVar(10.0f);
		_particleSystem.setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);

		// emits per frame
		_particleSystem.setEmissionRate(static_cast<float>(totalParticles) / life);

		// color of particles
		_particleSystem.setStartColor({ 0.76f, 0.25f, 0.12f, 1.0f });
		_particleSystem.setStartColorVar({ 0.0f, 0.0f, 0.0f, 0.0f });
		_particleSystem.setEndColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		_particleSystem.setEndColorVar({ 0.0f, 0.0f, 0.0f, 0.0f });

		_particleSystem.setPosVar({ 40.0f, 20.0f });
		break;
	}
	case FIREWORK:
	{
		int totalParticles{ 1500 };
		_particleSystem.initWithTotalParticles(totalParticles);

		// duration
		_particleSystem.setDuration(ParticleSystem::DURATION_INFINITY);

		// Gravity Mode
		_particleSystem.setEmitterMode(ParticleSystem::Mode::GRAVITY);

		// Gravity Mode: gravity
		_particleSystem.setGravity(Vec2(0.0f, 90.0f));

		// Gravity Mode: radial
		_particleSystem.setRadialAccel(0);
		_particleSystem.setRadialAccelVar(0);

		// Gravity Mode: speed of particles
		_particleSystem.setSpeed(-180.0f);
		_particleSystem.setSpeedVar(50.0f);

		// starting angle
		_particleSystem.setAngle(90.0f);
		_particleSystem.setAngleVar(20.0f);

		// life of particles
		float life{ 3.5f };
		_particleSystem.setLife(life);
		_particleSystem.setLifeVar(1.0f);

		// emits per frame
		_particleSystem.setEmissionRate(static_cast<float>(totalParticles) / life);

		// color of particles
		_particleSystem.setStartColor({ 0.5f, 0.5f, 0.5f, 1.0f });
		_particleSystem.setStartColorVar({ 0.5f, 0.5f, 0.5f, 0.1f });
		_particleSystem.setEndColor({ 0.1f, 0.1f, 0.1f, 0.2f });
		_particleSystem.setEndColorVar({ 0.1f, 0.1f, 0.1f, 0.2f });

		// size, in pixels
		_particleSystem.setStartSize(8.0f);
		_particleSystem.setStartSizeVar(2.0f);
		_particleSystem.setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);

		_particleSystem.setPosVar({ 0.0f, 0.0f });
		break;
	}
	case EXPLOSION:
	{
		int totalParticles{ 700 };
		_particleSystem.initWithTotalParticles(totalParticles);

		// duration
		float duration{ 0.1f };
		_particleSystem.setDuration(duration);

		// Gravity Mode
		_particleSystem.setEmitterMode(ParticleSystem::Mode::GRAVITY);

		// Gravity Mode: gravity
		_particleSystem.setGravity(Vec2(0.0f, 0.0f));

		// Gravity Mode: speed of particles
		_particleSystem.setSpeed(-70.0f);
		_particleSystem.setSpeedVar(40.0f);

		// Gravity Mode: tangential
		_particleSystem.setTangentialAccel(0);
		_particleSystem.setTangentialAccelVar(0);

		// starting angle
		_particleSystem.setAngle(90.0f);
		_particleSystem.setAngleVar(360.0f);

		// life of particles
		_particleSystem.setLife(5.0f);
		_particleSystem.setLifeVar(2.0f);

		// size, in pixels
		_particleSystem.setStartSize(15.0f);
		_particleSystem.setStartSizeVar(10.0f);
		_particleSystem.setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);

		// emits per frame
		_particleSystem.setEmissionRate(static_cast<float>(totalParticles) / duration);

		// color of particles
		_particleSystem.setStartColor({ 0.7f, 0.1f, 0.2f, 1.0f });
		_particleSystem.setStartColorVar({ 0.5f, 0.5f, 0.5f, 0.0f });
		_particleSystem.setEndColor({ 0.5f, 0.5f, 0.5f, 0.0f });
		_particleSystem.setEndColorVar({ 0.5f, 0.5f, 0.5f, 0.0f });

		_particleSystem.setPosVar({ 0.0f, 0.0f });
		break;
	}
	default:
		break;
	}
}
void ParticleObject::reset()
{
	_particleSystem.resetSystem();
}
