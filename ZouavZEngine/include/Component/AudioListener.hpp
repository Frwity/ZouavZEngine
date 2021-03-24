#pragma once
#include "Component.hpp"

class AudioListener : public Component
{
public:
	AudioListener() = default;
	AudioListener(class GameObject* _gameObject);

	~AudioListener() = default;

	void Update();

	void Editor() override;
};