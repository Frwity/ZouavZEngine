#pragma once
#include "Component.hpp"
#include "Sound.hpp"
#include "System/ResourcesManager.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class ResourceManager;

class AudioBroadcaster : public Component
{
private:
	Sound* sound = nullptr;
	bool updatePosition = true;

public:

	AudioBroadcaster() = delete;
	AudioBroadcaster(class GameObject* _gameObject, class Sound* _sound);

	~AudioBroadcaster() = default;

	void Update();
	void Play();
	void Stop();
	
	void SetLooping(bool _loop);
	void SetAmbient(bool _ambient);

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(updatePosition, sound->GetName());
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<AudioBroadcaster>& _construct)
	{
		std::string soundName;
		bool _updatePosition;
		_ar(_updatePosition, soundName);

		_construct(nullptr, ResourcesManager::GetResource<Sound>(soundName));
		_construct->updatePosition = _updatePosition;
	}
};


CEREAL_REGISTER_TYPE(AudioBroadcaster)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AudioBroadcaster)