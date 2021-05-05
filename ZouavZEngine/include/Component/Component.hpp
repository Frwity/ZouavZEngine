#pragma once

#include <vector>
#include <string>
#include <functional>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/access.hpp"

enum class E_COMPONENT
{
	AUDIO_BROADCASTER,
	AUDIO_LISTENER,
	LIGHT,
	MESHRENDERER,
	BOX_COLLISION,
	CAPSULE_COLLISION,
	SPHERE_COLLISION,
	PLANE,
	RIGID_BODY,
	RIGID_STATIC,
	CAMERA,
	NUMBER_OF_COMPONENTS
};

class Component
{
private:
	class GameObject* gameObject;
public:

	Component() = delete;
	Component(class GameObject* _gameObject);
	virtual ~Component() = default;

	virtual void Editor();

	virtual const char* GetComponentName() = 0;

	static bool EditorCollapsingHeader(const char* _name = "Component", std::function<void()> _f = [](){});

	void DeleteFromGameObject();

	GameObject& GetGameObject() { return *gameObject; }
	const GameObject& GetGameObject() const { return *gameObject; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Component>& _construct);
};
