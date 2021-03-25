#pragma once

#include <vector>
#include <string>

enum class E_COMPONENT
{
	AUDIO_BROADCASTER,
	AUDIO_LISTENER,
	LIGHT,
	MESHRENDERER,
	NUMBER_OF_COMPONENTS
};

class Component
{
public:
	// TODO : not private
	class GameObject* gameObject;

	Component(class GameObject* _gameObject);
	Component() = delete;
	virtual ~Component() = default;

	virtual void Editor();
};