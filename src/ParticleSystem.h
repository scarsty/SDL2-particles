#pragma once

//移植自Cocos2dx，版权声明请查看licenses文件夹

#include <vector>
#include <string>
#include <cmath>

struct Pointf
{
 public:
	Pointf() = default;

	Pointf(float x, float y)
		: _x(x), _y(y)
	{
	}
	~Pointf() = default;

	float _x = 0, _y = 0;

	Pointf operator*(float f) const
	{
		Pointf p{ _x * f, _y * f };
		return p;
	}

	[[nodiscard]] float getAngle() const
	{
		return atan2f(_y, _x);
	}
};

typedef Pointf Vec2;

struct Color4F
{
	float _r = 0, _g = 0, _b = 0, _a = 0;
};

class ParticleData
{
 public:
	float _posx = 0;
	float _posy = 0;
	float _startPosX = 0;
	float _startPosY = 0;

	float _colorR = 0;
	float _colorG = 0;
	float _colorB = 0;
	float _colorA = 0;

	float _deltaColorR = 0;
	float _deltaColorG = 0;
	float _deltaColorB = 0;
	float _deltaColorA = 0;

	float _size = 0;
	float _deltaSize = 0;
	float _rotation = 0;
	float _deltaRotation = 0;
	float _timeToLive = 0;
	unsigned int _atlasIndex = 0;

	//! Mode A: gravity, direction, radial accel, tangential accel
	struct
	{
		float _dirX = 0;
		float _dirY = 0;
		float _radialAccel = 0;
		float _tangentialAccel = 0;
	} _modeA;

	//! Mode B: radius mode
	struct
	{
		float _angle = 0;
		float _degreesPerSecond = 0;
		float _radius = 0;
		float _deltaRadius = 0;
	} _modeB;
};

//typedef void (*CC_UPDATE_PARTICLE_IMP)(id, SEL, tParticle*, Vec2);

/** @class ParticleSystem
 * @brief Particle System base class.
Attributes of a Particle System:
- emission rate of the particles
- Gravity Mode (Mode A):
- gravity
- direction
- speed +-  variance
- tangential acceleration +- variance
- radial acceleration +- variance
- Radius Mode (Mode B):
- startRadius +- variance
- endRadius +- variance
- rotate +- variance
- Properties common to all modes:
- life +- life variance
- start spin +- variance
- end spin +- variance
- start size +- variance
- end size +- variance
- start color +- variance
- end color +- variance
- life +- variance
- blending function
- texture

@code
emitter.radialAccel = 15;
emitter.startSpin = 0;
@endcode

*/
class ParticleSystem
{
 public:
	enum class Mode
	{
		GRAVITY,
		RADIUS,
	};

	enum
	{
		/** The Particle emitter lives forever. */
		DURATION_INFINITY = -1,

		/** The starting size of the particle is equal to the ending size. */
		START_SIZE_EQUAL_TO_END_SIZE = -1,

		/** The starting radius of the particle is equal to the ending radius. */
		START_RADIUS_EQUAL_TO_END_RADIUS = -1,
	};

 public:
	ParticleData getParticleData(std::size_t index);

	void addParticles(int count);

	void stopSystem();
	/** Kill all living particles.
	 */
	void resetSystem();
	/** Whether or not the system is full.
	 *
	 * @return True if the system is full.
	 */
	[[nodiscard]] bool isFull() const;

	/** Whether or not the particle system removed self on finish.
	 *
	 * @return True if the particle system removed self on finish.
	 */
	[[nodiscard]] virtual bool isAutoRemoveOnFinish() const;

	/** Set the particle system auto removed it self on finish.
	 *
	 * @param var True if the particle system removed self on finish.
	 */
	virtual void
	setAutoRemoveOnFinish(bool var);

	// mode A
	/** Gets the gravity.
	 *
	 * @return The gravity.
	 */
	virtual const Vec2& getGravity();

	/** Sets the gravity.
	 *
	 * @param g The gravity.
	 */
	virtual void setGravity(const Vec2& g);

	/** Gets the speed.
	 *
	 * @return The speed.
	 */
	[[nodiscard]] virtual float getSpeed() const;

	/** Sets the speed.
	 *
	 * @param speed The speed.
	 */
	virtual void setSpeed(float speed);

	/** Gets the speed variance.
	 *
	 * @return The speed variance.
	 */
	[[nodiscard]] virtual float getSpeedVar() const;

	/** Sets the speed variance.
	 *
	 * @param speed The speed variance.
	 */
	virtual void setSpeedVar(float speed);

	/** Gets the tangential acceleration.
	 *
	 * @return The tangential acceleration.
	 */
	[[nodiscard]] virtual float getTangentialAccel() const;

	/** Sets the tangential acceleration.
	 *
	 * @param t The tangential acceleration.
	 */
	virtual void setTangentialAccel(float t);

	/** Gets the tangential acceleration variance.
	 *
	 * @return The tangential acceleration variance.
	 */
	[[nodiscard]] virtual float getTangentialAccelVar() const;

	/** Sets the tangential acceleration variance.
	 *
	 * @param t The tangential acceleration variance.
	 */
	virtual void setTangentialAccelVar(float t);

	/** Gets the radial acceleration.
	 *
	 * @return The radial acceleration.
	 */
	[[nodiscard]] virtual float getRadialAccel() const;

	/** Sets the radial acceleration.
	 *
	 * @param t The radial acceleration.
	 */
	virtual void setRadialAccel(float t);

	/** Gets the radial acceleration variance.
	 *
	 * @return The radial acceleration variance.
	 */
	[[nodiscard]] virtual float getRadialAccelVar() const;

	/** Sets the radial acceleration variance.
	 *
	 * @param t The radial acceleration variance.
	 */
	virtual void setRadialAccelVar(float t);

	/** Whether or not the rotation of each particle to its direction.
	 *
	 * @return True if the rotation is the direction.
	 */
	[[nodiscard]] virtual bool getRotationIsDir() const;

	/** Sets the rotation of each particle to its direction.
	 *
	 * @param t True if the rotation is the direction.
	 */
	virtual void setRotationIsDir(bool t);

	// mode B
	/** Gets the start radius.
	 *
	 * @return The start radius.
	 */
	[[nodiscard]] virtual float getStartRadius() const;

	/** Sets the start radius.
	 *
	 * @param startRadius The start radius.
	 */
	virtual void setStartRadius(float startRadius);

	/** Gets the start radius variance.
	 *
	 * @return The start radius variance.
	 */
	[[nodiscard]] virtual float getStartRadiusVar() const;

	/** Sets the start radius variance.
	 *
	 * @param startRadiusVar The start radius variance.
	 */
	virtual void setStartRadiusVar(float startRadiusVar);

	/** Gets the end radius.
	 *
	 * @return The end radius.
	 */
	[[nodiscard]] virtual float getEndRadius() const;

	/** Sets the end radius.
	 *
	 * @param endRadius The end radius.
	 */
	virtual void setEndRadius(float endRadius);
	/** Gets the end radius variance.
	 *
	 * @return The end radius variance.
	 */
	[[nodiscard]] virtual float getEndRadiusVar() const;

	/** Sets the end radius variance.
	 *
	 * @param endRadiusVar The end radius variance.
	 */
	virtual void setEndRadiusVar(float endRadiusVar);

	/** Gets the number of degrees to rotate a particle around the source pos per second.
	 *
	 * @return The number of degrees to rotate a particle around the source pos per second.
	 */
	[[nodiscard]] virtual float getRotatePerSecond() const;

	/** Sets the number of degrees to rotate a particle around the source pos per second.
	 *
	 * @param degrees The number of degrees to rotate a particle around the source pos per second.
	 */
	virtual void setRotatePerSecond(float degrees);

	/** Gets the rotate per second variance.
	 *
	 * @return The rotate per second variance.
	 */
	[[nodiscard]] virtual float getRotatePerSecondVar() const;

	/** Sets the rotate per second variance.
	 *
	 * @param degrees The rotate per second variance.
	 */
	virtual void setRotatePerSecondVar(float degrees);

	//virtual void setScale(float s);
	//virtual void setRotation(float newRotation);
	//virtual void setScaleX(float newScaleX);
	//virtual void setScaleY(float newScaleY);

	/** Whether or not the particle system is active.
	 *
	 * @return True if the particle system is active.
	 */
	[[nodiscard]] virtual bool isActive() const;

	/** Gets the index of system in batch node array.
	 *
	 * @return The index of system in batch node array.
	 */
	[[nodiscard]] int getAtlasIndex() const;

	/** Sets the index of system in batch node array.
	 *
	 * @param index The index of system in batch node array.
	 */
	void setAtlasIndex(int index);

	/** Gets the Quantity of particles that are being simulated at the moment.
	 *
	 * @return The Quantity of particles that are being simulated at the moment.
	 */
	[[nodiscard]] unsigned int getParticleCount() const;

	/** Gets how many seconds the emitter will run. -1 means 'forever'.
	 *
	 * @return The seconds that the emitter will run. -1 means 'forever'.
	 */
	[[nodiscard]] float getDuration() const;

	/** Sets how many seconds the emitter will run. -1 means 'forever'.
	 *
	 * @param duration The seconds that the emitter will run. -1 means 'forever'.
	 */
	void setDuration(float duration);

	/** Gets the source position of the emitter.
	 *
	 * @return The source position of the emitter.
	 */
	[[nodiscard]] const Vec2& getSourcePosition() const;

	/** Sets the source position of the emitter.
	 *
	 * @param pos The source position of the emitter.
	 */
	void setSourcePosition(const Vec2& pos);

	/** Gets the position variance of the emitter.
	 *
	 * @return The position variance of the emitter.
	 */
	[[nodiscard]] const Vec2& getPosVar() const;

	/** Sets the position variance of the emitter.
	 *
	 * @param pos The position variance of the emitter.
	 */
	void setPosVar(const Vec2& pos);

	/** Gets the life of each particle.
	 *
	 * @return The life of each particle.
	 */
	[[nodiscard]] float getLife() const;

	/** Sets the life of each particle.
	 *
	 * @param life The life of each particle.
	 */
	void setLife(float life);

	/** Gets the life variance of each particle.
	 *
	 * @return The life variance of each particle.
	 */
	[[nodiscard]] float getLifeVar() const;

	/** Sets the life variance of each particle.
	 *
	 * @param lifeVar The life variance of each particle.
	 */
	void setLifeVar(float lifeVar);

	/** Gets the angle of each particle.
	 *
	 * @return The angle of each particle.
	 */
	[[nodiscard]] float getAngle() const;

	/** Sets the angle of each particle.
	 *
	 * @param angle The angle of each particle.
	 */
	void setAngle(float angle);

	/** Gets the angle variance of each particle.
	 *
	 * @return The angle variance of each particle.
	 */
	[[nodiscard]] float getAngleVar() const;

	/** Sets the angle variance of each particle.
	 *
	 * @param angleVar The angle variance of each particle.
	 */
	void setAngleVar(float angleVar);

	/** Switch between different kind of emitter modes:
	 - kParticleModeGravity: uses gravity, speed, radial and tangential acceleration.
	 - kParticleModeRadius: uses radius movement + rotation.
	 *
	 * @return The mode of the emitter.
	 */
	[[nodiscard]] Mode getEmitterMode() const;

	/** Sets the mode of the emitter.
	 *
	 * @param mode The mode of the emitter.
	 */
	void setEmitterMode(Mode mode);

	/** Gets the start size in pixels of each particle.
	 *
	 * @return The start size in pixels of each particle.
	 */
	[[nodiscard]] float getStartSize() const;

	/** Sets the start size in pixels of each particle.
	 *
	 * @param startSize The start size in pixels of each particle.
	 */
	void setStartSize(float startSize);

	/** Gets the start size variance in pixels of each particle.
	 *
	 * @return The start size variance in pixels of each particle.
	 */
	[[nodiscard]] float getStartSizeVar() const;

	/** Sets the start size variance in pixels of each particle.
	 *
	 * @param sizeVar The start size variance in pixels of each particle.
	 */
	void setStartSizeVar(float sizeVar);

	/** Gets the end size in pixels of each particle.
	 *
	 * @return The end size in pixels of each particle.
	 */
	[[nodiscard]] float getEndSize() const;

	/** Sets the end size in pixels of each particle.
	 *
	 * @param endSize The end size in pixels of each particle.
	 */
	void setEndSize(float endSize);

	/** Gets the end size variance in pixels of each particle.
	 *
	 * @return The end size variance in pixels of each particle.
	 */
	[[nodiscard]] float getEndSizeVar() const;

	/** Sets the end size variance in pixels of each particle.
	 *
	 * @param sizeVar The end size variance in pixels of each particle.
	 */
	void setEndSizeVar(float sizeVar);

	/** Gets the start color of each particle.
	 *
	 * @return The start color of each particle.
	 */
	[[nodiscard]] const Color4F& getStartColor() const;

	/** Sets the start color of each particle.
	 *
	 * @param color The start color of each particle.
	 */
	void setStartColor(const Color4F& color);

	/** Gets the start color variance of each particle.
	 *
	 * @return The start color variance of each particle.
	 */
	[[nodiscard]] const Color4F& getStartColorVar() const;

	/** Sets the start color variance of each particle.
	 *
	 * @param color The start color variance of each particle.
	 */
	void setStartColorVar(const Color4F& color);

	/** Gets the end color and end color variation of each particle.
	 *
	 * @return The end color and end color variation of each particle.
	 */
	[[nodiscard]] const Color4F& getEndColor() const;

	/** Sets the end color and end color variation of each particle.
	 *
	 * @param color The end color and end color variation of each particle.
	 */
	void setEndColor(const Color4F& color);

	/** Gets the end color variance of each particle.
	 *
	 * @return The end color variance of each particle.
	 */
	[[nodiscard]] const Color4F& getEndColorVar() const;

	/** Sets the end color variance of each particle.
	 *
	 * @param color The end color variance of each particle.
	 */
	void setEndColorVar(const Color4F& color);

	/** Gets the start spin of each particle.
	 *
	 * @return The start spin of each particle.
	 */
	[[nodiscard]] float getStartSpin() const;

	/** Sets the start spin of each particle.
	 *
	 * @param spin The start spin of each particle.
	 */
	void setStartSpin(float spin);

	/** Gets the start spin variance of each particle.
	 *
	 * @return The start spin variance of each particle.
	 */
	[[nodiscard]] float getStartSpinVar() const;

	/** Sets the start spin variance of each particle.
	 *
	 * @param pinVar The start spin variance of each particle.
	 */
	void setStartSpinVar(float pinVar);

	/** Gets the end spin of each particle.
	 *
	 * @return The end spin of each particle.
	 */
	[[nodiscard]] float getEndSpin() const;

	/** Sets the end spin of each particle.
	 *
	 * @param endSpin The end spin of each particle.
	 */
	void setEndSpin(float endSpin);

	/** Gets the end spin variance of each particle.
	 *
	 * @return The end spin variance of each particle.
	 */
	[[nodiscard]] float getEndSpinVar() const;

	/** Sets the end spin variance of each particle.
	 *
	 * @param endSpinVar The end spin variance of each particle.
	 */
	void setEndSpinVar(float endSpinVar);

	/** Gets the emission rate of the particles.
	 *
	 * @return The emission rate of the particles.
	 */
	[[nodiscard]] float getEmissionRate() const;

	/** Sets the emission rate of the particles.
	 *
	 * @param rate The emission rate of the particles.
	 */
	void setEmissionRate(float rate);

	/** Gets the maximum particles of the system.
	 *
	 * @return The maximum particles of the system.
	 */
	[[nodiscard]] virtual int getTotalParticles() const;

	/** Sets the maximum particles of the system.
	 *
	 * @param totalParticles The maximum particles of the system.
	 */
	virtual void setTotalParticles(int totalParticles);

	/** does the alpha value modify color */
	void setOpacityModifyRgb(bool opacityModifyRgb);

	[[nodiscard]] bool isOpacityModifyRgb() const;

	void update();

	ParticleSystem();
	virtual ~ParticleSystem();

	/** initializes a ParticleSystem*/
	virtual bool initWithTotalParticles(int numberOfParticles);
	virtual void resetTotalParticles(int numberOfParticles);
	[[nodiscard]] virtual bool isPaused() const;
	virtual void pauseEmissions();
	virtual void resumeEmissions();

 protected:
	//virtual void updateBlendFunc();

 protected:
	/** whether or not the particles are using blend additive.
	 If enabled, the following blending function will be used.
	 @code
	 source blend function = GL_SRC_ALPHA;
	 dest blend function = GL_ONE;
	 @endcode
	 */
	bool _isBlendAdditive = true;

	/** whether or not the node will be auto-removed when it has no particles left.
	By default it is false.
	@since v0.8
	*/
	bool _isAutoRemoveOnFinish = false;

	std::string _plistFile;
	//! time elapsed since the start of the system (in seconds)
	float _elapsed = 0;

	// Different modes
	//! Mode A:Gravity + Tangential Accel + Radial Accel
	struct
	{
		/** Gravity value. Only available in 'Gravity' mode. */
		Vec2 _gravity;
		/** speed of each particle. Only available in 'Gravity' mode.  */
		float _speed = 0;
		/** speed variance of each particle. Only available in 'Gravity' mode. */
		float _speedVar = 0;
		/** tangential acceleration of each particle. Only available in 'Gravity' mode. */
		float _tangentialAccel = 0;
		/** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
		float _tangentialAccelVar = 0;
		/** radial acceleration of each particle. Only available in 'Gravity' mode. */
		float _radialAccel = 0;
		/** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
		float _radialAccelVar = 0;
		/** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
		bool _rotationIsDir = false;
	} _modeA;

	//! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
	struct
	{
		/** The starting radius of the particles. Only available in 'Radius' mode. */
		float _startRadius = 0;
		/** The starting radius variance of the particles. Only available in 'Radius' mode. */
		float _startRadiusVar = 0;
		/** The ending radius of the particles. Only available in 'Radius' mode. */
		float _endRadius = 0;
		/** The ending radius variance of the particles. Only available in 'Radius' mode. */
		float _endRadiusVar = 0;
		/** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
		float _rotatePerSecond = 0;
		/** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
		float _rotatePerSecondVar = 0;
	} _modeB;

	//particle data
	std::vector<ParticleData> _particleData;

	//Emitter name
	std::string _configName;

	// color modulate
	//    BOOL colorModulate;

	//! How many particles can be emitted per second
	float _emitCounter = 0;

	// Optimization
	//CC_UPDATE_PARTICLE_IMP    updateParticleImp;
	//SEL                        updateParticleSel;

	/** weak reference to the SpriteBatchNode that renders the Sprite */
	//ParticleBatchNode* _batchNode;

	// index of system in batch node array
	int _atlasIndex = 0;

	//true if scaled or rotated
	bool _transformSystemDirty = false;
	// Number of allocated particles
	int _allocatedParticles = 0;

	/** Is the emitter active */
	bool _isActive = true;

	/** Quantity of particles that are being simulated at the moment */
	int _particleCount = 0;

	/** How many seconds the emitter will run. -1 means 'forever' */
	float _duration = 0;
	/** sourcePosition of the emitter */
	Vec2 _sourcePosition;
	/** Position variance of the emitter */
	Vec2 _posVar;
	/** life, and life variation of each particle */
	float _life = 0;
	/** life variance of each particle */
	float _lifeVar = 0;
	/** angle and angle variation of each particle */
	float _angle = 0;
	/** angle variance of each particle */
	float _angleVar = 0;

	/** Switch between different kind of emitter modes:
	- kParticleModeGravity: uses gravity, speed, radial and tangential acceleration
	- kParticleModeRadius: uses radius movement + rotation
	*/
	Mode _emitterMode = Mode::GRAVITY;

	/** start size in pixels of each particle */
	float _startSize = 0;
	/** size variance in pixels of each particle */
	float _startSizeVar = 0;
	/** end size in pixels of each particle */
	float _endSize = 0;
	/** end size variance in pixels of each particle */
	float _endSizeVar = 0;
	/** start color of each particle */
	Color4F _startColor;
	/** start color variance of each particle */
	Color4F _startColorVar;
	/** end color and end color variation of each particle */
	Color4F _endColor;
	/** end color variance of each particle */
	Color4F _endColorVar;
	//* initial angle of each particle
	float _startSpin = 0;
	//* initial angle of each particle
	float _startSpinVar = 0;
	//* initial angle of each particle
	float _endSpin = 0;
	//* initial angle of each particle
	float _endSpinVar = 0;
	/** emission rate of the particles */
	float _emissionRate = 0;
	/** maximum particles of the system */
	int _totalParticles = 0;

	/** conforms to CocosNodeTexture protocol */
	//BlendFunc _blendFunc;
	/** does the alpha value modify color */
	bool _opacityModifyRgb = false;
	/** does FlippedY variance of each particle */
	float _yCoordFlipped = 1;

	/** particles movement type: Free or Grouped
	@since v0.8
	*/
	//PositionType _positionType;

	/** is the emitter paused */
	bool _paused = false;

	/** is sourcePosition compatible */
	bool _sourcePositionCompatible = false;

	float _x = 0, _y = 0;

 public:
	void setPosition(const Vec2& position)
	{
		_x = position._x;
		_y = position._y;
	}

	[[nodiscard]] Vec2 getPosition() const
	{
		return { _x, _y };
	}
};
