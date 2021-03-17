#pragma once
#include "Resource.hpp"
#include "Maths/Vec3.hpp"

class Sound : public Resource
{
private:
	unsigned int buffer = 0;
	unsigned int source = 0;

public:
	float volumeIntensity = 1.0f;

	Sound() = default;
	Sound(const char* _path);
	~Sound();

	void Play();
	void Stop();
	void SetPosition(const Vec3& _position);
	void SetLooping(bool _loop);
	void SetVolume(float volume);
	void SetAmbient(bool _ambient);
	void SetMaxDistance(float _maxDistance);
};