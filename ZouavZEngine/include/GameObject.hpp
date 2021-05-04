#pragma once

#include "Component/Transform.hpp"
#include "Component/Component.hpp"
#include <vector>
#include <list>
#include <memory>
#include <cereal/types/base_class.hpp>
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"

namespace cereal
{
	class JSONOutputArchive;
};

class GameObject : public Transform
{
private:
	friend class Editor;
	friend class Scene;
	friend class cereal::access;


	static bool destroyGameObject;
	static std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<Component>> components;
	std::vector<GameObject*> children;
	GameObject* parent{ nullptr };
	bool toDestroy{ false };

public:
	static GameObject* currentLoadedGameObject;

	std::string name;
	std::string tag;

	GameObject() = delete;
	GameObject(const std::string& _name);
	GameObject(const std::string& _name, const std::string& _tag);
	~GameObject() = default;

	void Destroy();

	template <class Archive>
	void load(Archive& _ar)
	{
		int nbChild;

		_ar(name, tag, nbChild, components,
			localPosition.x, localPosition.y, localPosition.z,
			localRotation.x, localRotation.y, localRotation.z, localRotation.w,
			localScale.x, localScale.y, localScale.z);

		currentLoadedGameObject = this;

		std::string childName;
		std::string childTag;
		int nbChild2;

		for (int i = 0; i < nbChild; ++i) // TODO compress recurss
		{
			_ar(childName, childTag, nbChild2);

			GameObject* gameobject = CreateGameObject(childName, childTag);
			currentLoadedGameObject = gameobject;

			_ar(gameobject->components,
				gameobject->localPosition.x, gameobject->localPosition.y, gameobject->localPosition.z,
				gameobject->localRotation.x, gameobject->localRotation.y, gameobject->localRotation.z, gameobject->localRotation.w,
				gameobject->localScale.x, gameobject->localScale.y, gameobject->localScale.z);


			loadRecurss(_ar, gameobject, nbChild2);
		}
	}

	template <class Archive>
	void loadRecurss(Archive& _ar, GameObject* _gameobject, int _nbChild)
	{
		std::string childName;
		std::string childTag;
		int nbChild2;

		for (int i = 0; i < _nbChild; ++i)
		{
			_ar(childName, childTag, nbChild2);

			GameObject* gameobject = CreateGameObject(childName, childTag);
			currentLoadedGameObject = gameobject;

			_ar(gameobject->components,
				gameobject->localPosition.x, gameobject->localPosition.y, gameobject->localPosition.z,
				gameobject->localRotation.x, gameobject->localRotation.y, gameobject->localRotation.z, gameobject->localRotation.w,
				gameobject->localScale.x, gameobject->localScale.y, gameobject->localScale.z);



			_gameobject->AddChild(gameobject);

			loadRecurss(_ar, gameobject, nbChild2);
		}
	}

	template <class Archive>
	void save(Archive& _ar) const
	{
		int nbChild = (int)children.size();
		_ar(name, tag, nbChild, components, 
			localPosition.x, localPosition.y, localPosition.z,
			localRotation.x, localRotation.y, localRotation.z, localRotation.w,
			localScale.x, localScale.y, localScale.z);

		for (const GameObject* child : children)
			child->save(_ar);
	}

	void CreatePrefab();

	static GameObject* CreateGameObject(const std::string& _name);
	static GameObject* CreateGameObject(const std::string& _name, const std::string& _tag);

	const std::vector<GameObject*>& GetChildren() const { return children; }
	const GameObject* GetParent() const { return parent; }

	bool IsChildOf(const GameObject* _gameObject) const;
	void AddChild(GameObject* _child);
	void SetParent(GameObject* _parent);
	void RemoveChild(GameObject* _child);

	template<typename T, typename... Args>
	T* AddComponent(Args&&... _args)
	{
		components.emplace_back(std::make_unique<T>(this, _args...));
		return static_cast<T*>(components.back().get());
	}

	template<typename T>
	T* GetComponent() const
	{
		for (const std::unique_ptr<Component>& component : components)
		{
			T* returnComponent = dynamic_cast<T*>(component.get());
			if (returnComponent)
				return returnComponent;
		}
		return nullptr;
	}

	void UpdateTransform(const class Mat4& _heritedTransform);

	const std::vector<std::unique_ptr<Component>>& GetComponents();

	void RemoveComponent(Component* _component);

	virtual void OnContact();

	static void DestroyGameObjectIfNeedTo();
	static GameObject* GetGameObjectByName(std::string _name);
	static std::vector<GameObject*> GetGameObjectsByName(std::string _name);
	static GameObject* GetGameObjectByTag(std::string _tag);
	static std::vector<GameObject*> GetGameObjectsByTag(std::string _tag);
	
};
