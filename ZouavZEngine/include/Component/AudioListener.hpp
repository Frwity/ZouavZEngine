#pragma once
#include "Component.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

class AudioListener : public Component
{
public:
	AudioListener() = delete;
	AudioListener(class GameObject* _gameObject);

	~AudioListener() = default;

	void Update();

	void Editor() override;

	const char* GetComponentName() override { return "AudioListener"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<AudioListener>& _construct);
};


CEREAL_REGISTER_TYPE(AudioListener)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AudioListener)
