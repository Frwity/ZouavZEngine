#pragma once
#include <vector>

class Vec3;

class SoundManager
{
private:
	static std::vector<class Sound*> sounds;

public:
	static float mainVolume;

	SoundManager() = delete;
	~SoundManager() = delete;

	static void Init();
	static void Update();
	static void Destroy();

	static void AddSound(class Sound* _newSound);
	static void RemoveSound(class Sound* _soundToRemove);
	
	static void SetListenerPosition(const Vec3& _position);
	static void SetListenerVelocity(const Vec3& _velocity);
};