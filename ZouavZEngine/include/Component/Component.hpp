#pragma once

#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/access.hpp"

class Component
{
private:

public:
	// TODO : not private
	class GameObject* gameObject;

	Component() = delete;
	Component(class GameObject* _gameObject);
	virtual ~Component() = default;

	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Component>& _construct)
	{
		_construct(nullptr);
	}
};