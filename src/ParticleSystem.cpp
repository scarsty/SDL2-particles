#include "ParticleSystem.h"
#include <algorithm>
#include <cmath>

inline float deg2Rad(float a)
{
	return a * 0.01745329252f;
}

inline float rad2Deg(float a)
{
	return a * 57.29577951f;
}

inline float clampf(float value, float minInclusive, float maxInclusive)
{
	if (minInclusive > maxInclusive)
	{
		std::swap(minInclusive, maxInclusive);
	}
	return value < minInclusive ? minInclusive : value < maxInclusive ? value : maxInclusive;
}

inline void normalizePoint(float x, float y, Pointf* out)
{
	float n = x * x + y * y;
	// Already normalized.
	if (n == 1.0f)
	{
		return;
	}

	n = std::sqrt(n);
	// Too close to zero.
	if (n < 1e-5)
	{
		return;
	}

	n = 1.0f / n;
	out->_x = x * n;
	out->_y = y * n;
}

/**
A more effect random number getter function, get from ejoy2d.
*/
inline static float randomM11(unsigned int* seed)
{
	*seed = *seed * 134775813 + 1;
	union
	{
		uint32_t _d;
		float _f;
	} u{};
	u._d = (((uint32_t)(*seed) & 0x7fff) << 8) | 0x40000000;
	return u._f - 3.0f;
}

ParticleSystem::ParticleSystem() = default;

// implementation ParticleSystem

bool ParticleSystem::initWithTotalParticles(int numberOfParticles)
{
	_totalParticles = numberOfParticles;
	_isActive = true;
	_emitterMode = Mode::GRAVITY;
	_isAutoRemoveOnFinish = false;
	_transformSystemDirty = false;

	resetTotalParticles(numberOfParticles);

	return true;
}

void ParticleSystem::resetTotalParticles(int numberOfParticles)
{
	if (_particleData.size() < numberOfParticles)
	{
		_particleData.resize(numberOfParticles);
	}
}

ParticleSystem::~ParticleSystem() = default;

void ParticleSystem::addParticles(int count)
{
	if (_paused)
	{
		return;
	}
	uint32_t randseed = std::rand();

	int start = _particleCount;
	_particleCount += count;

	//life
	for (int i = start; i < _particleCount; ++i)
	{
		float theLife = _life + _lifeVar * randomM11(&randseed);
		_particleData[i]._timeToLive = (std::max)(0.0f, theLife);
	}

	//position
	for (int i = start; i < _particleCount; ++i)
	{
		_particleData[i]._posx = _sourcePosition._x + _posVar._x * randomM11(&randseed);
	}

	for (int i = start; i < _particleCount; ++i)
	{
		_particleData[i]._posy = _sourcePosition._y + _posVar._y * randomM11(&randseed);
	}

	//color
#define SET_COLOR(c, b, v)                                                 \
    for (int i = start; i < _particleCount; ++i)                           \
    {                                                                      \
        _particleData[i].c = clampf((b) + (v) * randomM11(&randseed), 0, 1); \
    }

	SET_COLOR(_colorR, _startColor._r, _startColorVar._r)
	SET_COLOR(_colorG, _startColor._g, _startColorVar._g)
	SET_COLOR(_colorB, _startColor._b, _startColorVar._b)
	SET_COLOR(_colorA, _startColor._a, _startColorVar._a)

	SET_COLOR(_deltaColorR, _endColor._r, _endColorVar._r)
	SET_COLOR(_deltaColorG, _endColor._g, _endColorVar._g)
	SET_COLOR(_deltaColorB, _endColor._b, _endColorVar._b)
	SET_COLOR(_deltaColorA, _endColor._a, _endColorVar._a)

#define SET_DELTA_COLOR(c, dc)                                                                              \
    for (int i = start; i < _particleCount; ++i)                                                            \
    {                                                                                                       \
        _particleData[i].dc = (_particleData[i].dc - _particleData[i].c) / _particleData[i]._timeToLive; \
    }

	SET_DELTA_COLOR(_colorR, _deltaColorR)
	SET_DELTA_COLOR(_colorG, _deltaColorG)
	SET_DELTA_COLOR(_colorB, _deltaColorB)
	SET_DELTA_COLOR(_colorA, _deltaColorA)

	//size
	for (int i = start; i < _particleCount; ++i)
	{
		_particleData[i]._size = _startSize + _startSizeVar * randomM11(&randseed);
		_particleData[i]._size = (std::max)(0.0f, _particleData[i]._size);
	}

	if (_endSize != START_SIZE_EQUAL_TO_END_SIZE)
	{
		for (int i = start; i < _particleCount; ++i)
		{
			float endSize = _endSize + _endSizeVar * randomM11(&randseed);
			endSize = (std::max)(0.0f, endSize);
			_particleData[i]._deltaSize = (endSize - _particleData[i]._size) / _particleData[i]._timeToLive;
		}
	}
	else
	{
		for (int i = start; i < _particleCount; ++i)
		{
			_particleData[i]._deltaSize = 0.0f;
		}
	}

	// rotation
	for (int i = start; i < _particleCount; ++i)
	{
		_particleData[i]._rotation = _startSpin + _startSpinVar * randomM11(&randseed);
	}
	for (int i = start; i < _particleCount; ++i)
	{
		float endA = _endSpin + _endSpinVar * randomM11(&randseed);
		_particleData[i]._deltaRotation = (endA - _particleData[i]._rotation) / _particleData[i]._timeToLive;
	}

	// position
	Vec2 pos;
	pos._x = _x;
	pos._y = _y;

	for (int i = start; i < _particleCount; ++i)
	{
		_particleData[i]._startPosX = pos._x;
	}
	for (int i = start; i < _particleCount; ++i)
	{
		_particleData[i]._startPosY = pos._y;
	}

	// Mode Gravity: A
	if (_emitterMode == Mode::GRAVITY)
	{

		// radial accel
		for (int i = start; i < _particleCount; ++i)
		{
			_particleData[i]._modeA._radialAccel = _modeA._radialAccel + _modeA._radialAccelVar * randomM11(&randseed);
		}

		// tangential accel
		for (int i = start; i < _particleCount; ++i)
		{
			_particleData[i]._modeA._tangentialAccel =
				_modeA._tangentialAccel + _modeA._tangentialAccelVar * randomM11(&randseed);
		}

		// rotation is dir
		if (_modeA._rotationIsDir)
		{
			for (int i = start; i < _particleCount; ++i)
			{
				float a = deg2Rad(_angle + _angleVar * randomM11(&randseed));
				Vec2 v(cosf(a), sinf(a));
				float s = _modeA._speed + _modeA._speedVar * randomM11(&randseed);
				Vec2 dir = v * s;
				_particleData[i]._modeA._dirX = dir._x;    //v * s ;
				_particleData[i]._modeA._dirY = dir._y;
				_particleData[i]._rotation = -rad2Deg(dir.getAngle());
			}
		}
		else
		{
			for (int i = start; i < _particleCount; ++i)
			{
				float a = deg2Rad(_angle + _angleVar * randomM11(&randseed));
				Vec2 v(cosf(a), sinf(a));
				float s = _modeA._speed + _modeA._speedVar * randomM11(&randseed);
				Vec2 dir = v * s;
				_particleData[i]._modeA._dirX = dir._x;    //v * s ;
				_particleData[i]._modeA._dirY = dir._y;
			}
		}
	}

		// Mode Radius: B
	else
	{
		for (int i = start; i < _particleCount; ++i)
		{
			_particleData[i]._modeB._radius = _modeB._startRadius + _modeB._startRadiusVar * randomM11(&randseed);
		}

		for (int i = start; i < _particleCount; ++i)
		{
			_particleData[i]._modeB._angle = deg2Rad(_angle + _angleVar * randomM11(&randseed));
		}

		for (int i = start; i < _particleCount; ++i)
		{
			_particleData[i]._modeB._degreesPerSecond =
				deg2Rad(_modeB._rotatePerSecond + _modeB._rotatePerSecondVar * randomM11(&randseed));
		}

		if (_modeB._endRadius == START_RADIUS_EQUAL_TO_END_RADIUS)
		{
			for (int i = start; i < _particleCount; ++i)
			{
				_particleData[i]._modeB._deltaRadius = 0.0f;
			}
		}
		else
		{
			for (int i = start; i < _particleCount; ++i)
			{
				float endRadius = _modeB._endRadius + _modeB._endRadiusVar * randomM11(&randseed);
				_particleData[i]._modeB._deltaRadius =
					(endRadius - _particleData[i]._modeB._radius) / _particleData[i]._timeToLive;
			}
		}
	}
}

void ParticleSystem::stopSystem()
{
	_isActive = false;
	_elapsed = _duration;
	_emitCounter = 0.f;
}

void ParticleSystem::resetSystem()
{
	_isActive = true;
	_elapsed = 0.f;
	_emitCounter = 0.f;

//	for (int i = 0; i < _particleCount; ++i)
//	{
//		_particleData[i]._timeToLive = 0.f;
//	}
	_particleData.clear();
	_particleCount = 0;

	resetTotalParticles(_totalParticles);
}

bool ParticleSystem::isFull() const
{
	return (_particleCount == _totalParticles);
}

// ParticleSystem - MainLoop
void ParticleSystem::update()
{
	float dt = 1.0 / 25;
	if (_isActive && _emissionRate != 0)
	{
		float rate = 1.0f / _emissionRate;
		int totalParticles = _totalParticles;

		//issue #1201, prevent bursts of particles, due to too high emitCounter
		if (_particleCount < totalParticles)
		{
			_emitCounter += dt;
			if (_emitCounter < 0.f)
			{
				_emitCounter = 0.f;
			}
		}

		int emitCount = (std::min)(1.0f * (totalParticles - _particleCount), _emitCounter / rate);
		addParticles(emitCount);
		_emitCounter -= rate * emitCount;

		_elapsed += dt;
		if (_elapsed < 0.f)
		{
			_elapsed = 0.f;
		}
		if (_duration != DURATION_INFINITY && _duration < _elapsed)
		{
			this->stopSystem();
		}
	}

	for (int i = 0; i < _particleCount; ++i)
	{
		_particleData[i]._timeToLive -= dt;
	}

	// rebirth
	for (int i = 0; i < _particleCount; ++i)
	{
		if (_particleData[i]._timeToLive <= 0.0f)
		{
			//int j = _particleCount - 1;
			//while (j > 0 && particle_data_[i].timeToLive <= 0)
			//{
			//    _particleCount--;
			//    j--;
			//}
			_particleData[i] = _particleData[_particleCount - 1];
			--_particleCount;
		}
	}

	if (_emitterMode == Mode::GRAVITY)
	{
		for (int i = 0; i < _particleCount; ++i)
		{
			Pointf tmp, radial = { 0.0f, 0.0f }, tangential;

			// radial acceleration
			if (_particleData[i]._posx != 0 || _particleData[i]._posy != 0)
			{
				normalizePoint(_particleData[i]._posx, _particleData[i]._posy, &radial);
			}
			tangential = radial;
			radial._x *= _particleData[i]._modeA._radialAccel;
			radial._y *= _particleData[i]._modeA._radialAccel;

			// tangential acceleration
			std::swap(tangential._x, tangential._y);
			tangential._x *= -_particleData[i]._modeA._tangentialAccel;
			tangential._y *= _particleData[i]._modeA._tangentialAccel;

			// (gravity + radial + tangential) * dt
			tmp._x = radial._x + tangential._x + _modeA._gravity._x;
			tmp._y = radial._y + tangential._y + _modeA._gravity._y;
			tmp._x *= dt;
			tmp._y *= dt;

			_particleData[i]._modeA._dirX += tmp._x;
			_particleData[i]._modeA._dirY += tmp._y;

			// this is cocos2d-x v3.0
			// if (_configName.length()>0 && _yCoordFlipped != -1)

			// this is cocos2d-x v3.0
			tmp._x = _particleData[i]._modeA._dirX * dt * _yCoordFlipped;
			tmp._y = _particleData[i]._modeA._dirY * dt * _yCoordFlipped;
			_particleData[i]._posx += tmp._x;
			_particleData[i]._posy += tmp._y;
		}
	}
	else
	{
		for (int i = 0; i < _particleCount; ++i)
		{
			_particleData[i]._modeB._angle += _particleData[i]._modeB._degreesPerSecond * dt;
			_particleData[i]._modeB._radius += _particleData[i]._modeB._deltaRadius * dt;
			_particleData[i]._posx = -cosf(_particleData[i]._modeB._angle) * _particleData[i]._modeB._radius;
			_particleData[i]._posy =
				-sinf(_particleData[i]._modeB._angle) * _particleData[i]._modeB._radius * _yCoordFlipped;
		}
	}

	//color, size, rotation
	for (int i = 0; i < _particleCount; ++i)
	{
		_particleData[i]._colorR += _particleData[i]._deltaColorR * dt;
		_particleData[i]._colorG += _particleData[i]._deltaColorG * dt;
		_particleData[i]._colorB += _particleData[i]._deltaColorB * dt;
		_particleData[i]._colorA += _particleData[i]._deltaColorA * dt;
		_particleData[i]._size += (_particleData[i]._deltaSize * dt);
		_particleData[i]._size = (std::max)(0.0f, _particleData[i]._size);
		_particleData[i]._rotation += _particleData[i]._deltaRotation * dt;
	}
}

// ParticleSystem - Properties of Gravity Mode
void ParticleSystem::setTangentialAccel(float t)
{
	_modeA._tangentialAccel = t;
}

float ParticleSystem::getTangentialAccel() const
{
	return _modeA._tangentialAccel;
}

void ParticleSystem::setTangentialAccelVar(float t)
{
	_modeA._tangentialAccelVar = t;
}

float ParticleSystem::getTangentialAccelVar() const
{
	return _modeA._tangentialAccelVar;
}

void ParticleSystem::setRadialAccel(float t)
{
	_modeA._radialAccel = t;
}

float ParticleSystem::getRadialAccel() const
{
	return _modeA._radialAccel;
}

void ParticleSystem::setRadialAccelVar(float t)
{
	_modeA._radialAccelVar = t;
}

float ParticleSystem::getRadialAccelVar() const
{
	return _modeA._radialAccelVar;
}

void ParticleSystem::setRotationIsDir(bool t)
{
	_modeA._rotationIsDir = t;
}

bool ParticleSystem::getRotationIsDir() const
{
	return _modeA._rotationIsDir;
}

void ParticleSystem::setGravity(const Vec2& g)
{
	_modeA._gravity = g;
}

const Vec2& ParticleSystem::getGravity()
{
	return _modeA._gravity;
}

void ParticleSystem::setSpeed(float speed)
{
	_modeA._speed = speed;
}

float ParticleSystem::getSpeed() const
{
	return _modeA._speed;
}

void ParticleSystem::setSpeedVar(float speedVar)
{

	_modeA._speedVar = speedVar;
}

float ParticleSystem::getSpeedVar() const
{

	return _modeA._speedVar;
}

// ParticleSystem - Properties of Radius Mode
void ParticleSystem::setStartRadius(float startRadius)
{
	_modeB._startRadius = startRadius;
}

float ParticleSystem::getStartRadius() const
{
	return _modeB._startRadius;
}

void ParticleSystem::setStartRadiusVar(float startRadiusVar)
{
	_modeB._startRadiusVar = startRadiusVar;
}

float ParticleSystem::getStartRadiusVar() const
{
	return _modeB._startRadiusVar;
}

void ParticleSystem::setEndRadius(float endRadius)
{
	_modeB._endRadius = endRadius;
}

float ParticleSystem::getEndRadius() const
{
	return _modeB._endRadius;
}

void ParticleSystem::setEndRadiusVar(float endRadiusVar)
{
	_modeB._endRadiusVar = endRadiusVar;
}

float ParticleSystem::getEndRadiusVar() const
{

	return _modeB._endRadiusVar;
}

void ParticleSystem::setRotatePerSecond(float degrees)
{
	_modeB._rotatePerSecond = degrees;
}

float ParticleSystem::getRotatePerSecond() const
{
	return _modeB._rotatePerSecond;
}

void ParticleSystem::setRotatePerSecondVar(float degrees)
{
	_modeB._rotatePerSecondVar = degrees;
}

float ParticleSystem::getRotatePerSecondVar() const
{
	return _modeB._rotatePerSecondVar;
}

bool ParticleSystem::isActive() const
{
	return _isActive;
}

int ParticleSystem::getTotalParticles() const
{
	return _totalParticles;
}

void ParticleSystem::setTotalParticles(int var)
{
	_totalParticles = var;
}

bool ParticleSystem::isAutoRemoveOnFinish() const
{
	return _isAutoRemoveOnFinish;
}

void ParticleSystem::setAutoRemoveOnFinish(bool var)
{
	_isAutoRemoveOnFinish = var;
}

////don't use a transform matrix, this is faster
//void ParticleSystem::setScale(float s)
//{
//    _transformSystemDirty = true;
//    Node::setScale(s);
//}
//
//void ParticleSystem::setRotation(float newRotation)
//{
//    _transformSystemDirty = true;
//    Node::setRotation(newRotation);
//}
//
//void ParticleSystem::setScaleX(float newScaleX)
//{
//    _transformSystemDirty = true;
//    Node::setScaleX(newScaleX);
//}
//
//void ParticleSystem::setScaleY(float newScaleY)
//{
//    _transformSystemDirty = true;
//    Node::setScaleY(newScaleY);
//}

bool ParticleSystem::isPaused() const
{
	return _paused;
}

void ParticleSystem::pauseEmissions()
{
	_paused = true;
}

void ParticleSystem::resumeEmissions()
{
	_paused = false;
}

ParticleData ParticleSystem::getParticleData(std::size_t index)
{
	return _particleData[index];
}

int ParticleSystem::getAtlasIndex() const
{
	return _atlasIndex;
}

unsigned int ParticleSystem::getParticleCount() const
{
	return _particleCount;
}

float ParticleSystem::getDuration() const
{
	return _duration;
}

float ParticleSystem::getEmissionRate() const
{
	return _emissionRate;
}

void ParticleSystem::setEmissionRate(float rate)
{
	_emissionRate = rate;
}

bool ParticleSystem::isOpacityModifyRgb() const
{
	return _opacityModifyRgb;
}

void ParticleSystem::setOpacityModifyRgb(bool opacityModifyRgb)
{
	_opacityModifyRgb = opacityModifyRgb;
}

void ParticleSystem::setEndSpinVar(float endSpinVar)
{
	_endSpinVar = endSpinVar;
}

float ParticleSystem::getEndSpinVar() const
{
	return _endSpinVar;
}

float ParticleSystem::getStartSpinVar() const
{
	return _startSpinVar;
}

void ParticleSystem::setStartSpin(float spin)
{
	_startSpin = spin;
}

float ParticleSystem::getStartSpin() const
{
	return _startSpin;
}

void ParticleSystem::setEndColorVar(const Color4F& color)
{
	_endColorVar = color;
}

const Color4F& ParticleSystem::getEndColorVar() const
{
	return _endColorVar;
}

void ParticleSystem::setEndColor(const Color4F& color)
{
	_endColor = color;
}

const Color4F& ParticleSystem::getEndColor() const
{
	return _endColor;
}

void ParticleSystem::setStartColorVar(const Color4F& color)
{
	_startColorVar = color;
}

const Color4F& ParticleSystem::getStartColorVar() const
{
	return _startColorVar;
}

void ParticleSystem::setStartColor(const Color4F& color)
{
	_startColor = color;
}

const Color4F& ParticleSystem::getStartColor() const
{
	return _startColor;
}

float ParticleSystem::getEndSizeVar() const
{
	return _endSizeVar;
}

void ParticleSystem::setEndSizeVar(float sizeVar)
{
	_endSizeVar = sizeVar;
}

void ParticleSystem::setEndSize(float endSize)
{
	_endSize = endSize;
}

float ParticleSystem::getEndSize() const
{
	return _endSize;
}

void ParticleSystem::setEndSpin(float endSpin)
{
	_endSpin = endSpin;
}

float ParticleSystem::getEndSpin() const
{
	return _endSpin;
}

void ParticleSystem::setStartSpinVar(float pinVar)
{
	_startSpinVar = pinVar;
}

void ParticleSystem::setStartSizeVar(float sizeVar)
{
	_startSizeVar = sizeVar;
}

float ParticleSystem::getStartSizeVar() const
{
	return _startSizeVar;
}

void ParticleSystem::setStartSize(float startSize)
{
	_startSize = startSize;
}

float ParticleSystem::getStartSize() const
{
	return _startSize;
}

void ParticleSystem::setEmitterMode(ParticleSystem::Mode mode)
{
	_emitterMode = mode;
}

ParticleSystem::Mode ParticleSystem::getEmitterMode() const
{
	return _emitterMode;
}

void ParticleSystem::setAngleVar(float angleVar)
{
	_angleVar = angleVar;
}

float ParticleSystem::getAngleVar() const
{
	return _angleVar;
}

void ParticleSystem::setAngle(float angle)
{
	_angle = angle;
}

float ParticleSystem::getAngle() const
{
	return _angle;
}

void ParticleSystem::setLifeVar(float lifeVar)
{
	_lifeVar = lifeVar;
}

float ParticleSystem::getLifeVar() const
{
	return _lifeVar;
}

void ParticleSystem::setLife(float life)
{
	_life = life;
}

float ParticleSystem::getLife() const
{
	return _life;
}

void ParticleSystem::setPosVar(const Vec2& pos)
{
	_posVar = pos;
}

const Vec2& ParticleSystem::getPosVar() const
{
	return _posVar;
}

void ParticleSystem::setSourcePosition(const Vec2& pos)
{
	_sourcePosition = pos;
}

const Vec2& ParticleSystem::getSourcePosition() const
{
	return _sourcePosition;
}

void ParticleSystem::setDuration(float duration)
{
	_duration = duration;
}

void ParticleSystem::setAtlasIndex(int index)
{
	_atlasIndex = index;
}
