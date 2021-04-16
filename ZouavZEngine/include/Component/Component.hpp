#pragma once

#include <vector>
#include <string>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/access.hpp"

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

	Component() = delete;
	Component(class GameObject* _gameObject);
	virtual ~Component() = default;

	virtual void Editor();
	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Component>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}
};
