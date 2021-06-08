#pragma once
#include <vector>

class Vec3;

class SoundManager
{
private:
	static std::vector<class AudioBroadcaster*> audioBroadcasters;
	static class AudioListener* audioListener;

public:
	static float mainVolume;

	SoundManager() = delete;
	~SoundManager() = delete;

	static void Init();
	static void Update();
	static void Destroy();

	static void AddSound(class AudioBroadcaster* _newSound);
	static void RemoveSound(class AudioBroadcaster* _soundToRemove);

	static void SetListener(class AudioListener* _listener);
	static void RemoveListener(class AudioListener* _listener);

	static void SetListenerPosition(const Vec3& _position);
	static void SetListenerVelocity(const Vec3& _velocity);
};