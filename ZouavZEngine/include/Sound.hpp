#pragma once
#include "Resource.hpp"
#include "Maths/Vec3.hpp"

class Sound : public Resource
{
private:
	unsigned int buffer;
	unsigned int source;

public:
	Sound() = default;
	Sound(const char* _path);
	~Sound();

	void Play();
	void Stop();
	void SetPosition(const Vec3& _position);
	void SetLooping(bool _loop);
};