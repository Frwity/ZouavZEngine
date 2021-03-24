#pragma once
#include "Component.hpp"

class AudioBroadcaster : public Component
{
private:
	class Sound* sound = nullptr;
	bool ambient = false;
	bool loop = false;

public:
	AudioBroadcaster() = default;
	AudioBroadcaster(class GameObject* _gameObject);
	AudioBroadcaster(class GameObject* _gameObject, class Sound* _sound);

	~AudioBroadcaster() = default;

	void Update();
	void Play();
	void Stop();
	
	void SetLooping(bool _loop);
	void SetAmbient(bool _ambient);

	void Editor() override;
};