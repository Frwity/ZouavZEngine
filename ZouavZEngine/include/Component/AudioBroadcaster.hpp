#pragma once
#include "Component.hpp"
#include "Sound.hpp"
#include "System/ResourcesManager.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class ResourceManager;

class Vec3;

class AudioBroadcaster : public Component
{
private:
	class Sound* sound = nullptr;
	unsigned int source = 0;
	bool ambient = false;
	bool loop = false;

public:
	float volumeIntensity = 1.0f;

	AudioBroadcaster() = delete;
	AudioBroadcaster(class GameObject* _gameObject);
	AudioBroadcaster(class GameObject* _gameObject, class Sound* _sound);

	~AudioBroadcaster();

	void Update();
	void Play();
	void Stop();
	
	void SetPosition(const Vec3& _position);
	void SetVolume(float volume);
	void SetMaxDistance(float _maxDistance);
	void SetLooping(bool _loop);
	void SetAmbient(bool _ambient);

	void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(ambient, loop, sound->GetName());
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<AudioBroadcaster>& _construct)
	{
		std::string soundName;
		bool _ambient;
		bool _loop;
		_ar(_ambient, _loop, soundName);

		_construct(nullptr, ResourcesManager::GetResource<Sound>(soundName));
		_construct->ambient = _ambient;
		_construct->loop = _loop;
	}
};


CEREAL_REGISTER_TYPE(AudioBroadcaster)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AudioBroadcaster)
