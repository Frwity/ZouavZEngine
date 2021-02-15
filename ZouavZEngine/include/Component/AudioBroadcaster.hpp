#pragma once
#include "Component.hpp"

class AudioBroadcaster : public Component
{
private:
	class Sound* sound = nullptr;

public:
	bool updatePosition = true;

	AudioBroadcaster() = default;
	AudioBroadcaster(class GameObject* _gameObject, class Sound* _sound);

	~AudioBroadcaster() = default;

	void Update();
	void Play();
	void Stop();
	
	void SetLooping(bool _loop);
};