#pragma once

#include <vector>
#include <string>
#include <functional>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/access.hpp"
#include <cereal/types/base_class.hpp>

enum class E_COMPONENT
{
	AUDIO_BROADCASTER,
	AUDIO_LISTENER,
	LIGHT,
	MESHRENDERER,
	FONTCOMPONENT,
	BOX_COLLISION,
	CAPSULE_COLLISION,
	SPHERE_COLLISION,
	PLANE,
	RIGID_BODY,
	RIGID_STATIC,
	CAMERA,
	PLAYER,
	SKYBOX,
	NUMBER_OF_COMPONENTS
};

class Component
{
protected:
	friend class Editor;
	friend class GameObject;
	class GameObject* gameObject;
	bool isActive = true;

	virtual void InternalActivate() {}
	virtual void InternalDehactivate() {}
public:

	Component() = delete;
	Component(class GameObject* _gameObject);
	Component(const Component& _other);
	virtual Component* Clone() const = 0;
	virtual ~Component() = default;

	virtual void Editor();

	virtual const char* GetComponentName() = 0;

	static bool EditorCollapsingHeader(const char* _name = "Component", std::function<void()> _f = [](){});

	void DeleteFromGameObject();

	bool IsActive() const;
	virtual void Activate() { isActive = true; }
	virtual void Dehactivate() { isActive = false; }

	GameObject& GetGameObject() { return *gameObject; }
	const GameObject& GetGameObject() const { return *gameObject; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(isActive);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Component>& _construct);
};

