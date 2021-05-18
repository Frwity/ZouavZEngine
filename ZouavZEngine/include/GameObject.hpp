#pragma once

#include "Component/Transform.hpp"
#include "Component/Component.hpp"

#include <cereal/types/base_class.hpp>
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace cereal
{
	class JSONOutputArchive;
};

class GameObject : public Transform
{
private:
	friend class Editor;
	friend class Scene;
	friend class Prefab;
	friend class cereal::access;

	static bool destroyGameObject;
	static std::vector<std::unique_ptr<GameObject>> gameObjects;
	static std::unordered_map<std::string ,std::unique_ptr<GameObject>> prefabGameObjects;
	std::vector<std::unique_ptr<Component>> components;
	std::vector<GameObject*> children;
	GameObject* parent{ nullptr };
	bool isActive{ true };
	bool toDestroy{ false };
	bool isPrefab{ false };

	std::string name;
	std::string tag;

	void CreatePrefab();
	static GameObject* LoadPrefab(const std::string& _path);
public:
	static GameObject* currentLoadedGameObject;

	GameObject() = delete;
	GameObject(const std::string& _name);
	GameObject(const std::string& _name, const std::string& _tag);
	GameObject& operator=(const GameObject&);
	GameObject(const GameObject&);
	GameObject(GameObject&&) = default;
	~GameObject() = default;

	void Destroy();

	const std::string& GetName() const { return name; }
	void SetName(const std::string& _newName) { name = _newName; }
	const std::string& GetTag() const { return tag; }
	void SetTag(const std::string& _newTag) { tag = _newTag; }

	bool IsActive() const { return isActive && !isPrefab; }
	void Activate();
	void Dehactivate();

	static GameObject* Instanciate(GameObject* _gameobject);
	static GameObject* Instanciate(GameObject* _gameobject, const Vec3& _position);
	static GameObject* Instanciate(GameObject* _gameobject, GameObject* _parent);
	static GameObject* Instanciate(GameObject* _gameobject, GameObject* _parent, const Vec3& _position);

	static GameObject* CreateGameObject(const std::string& _name);
	static GameObject* CreateGameObject(const std::string& _name, const std::string& _tag);

	const std::vector<GameObject*>& GetChildren() const { return children; }
	GameObject& GetParent() const { return *parent; }

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

	template<typename T>
	std::vector<T*> GetComponents() // sa se trouve ca marche pas hihi TODO ENLEVER CE TODO
	{
		std::vector<T*> returnComponents;
		for (const std::unique_ptr<Component>& component : components)
		{
			T* component = dynamic_cast<T*>(component.get());
			if (component)
				returnComponents.push_back(component);
		}
		return returnComponents;
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

	template <class Archive>
	void load(Archive& _ar)
	{
		int nbChild;

		currentLoadedGameObject = this;

		_ar(name, tag, nbChild, components,
			localPosition.x, localPosition.y, localPosition.z,
			localRotation.x, localRotation.y, localRotation.z, localRotation.w,
			localScale.x, localScale.y, localScale.z, isActive, isPrefab);

		std::string childName;
		std::string childTag;
		int nbChild2;

		for (int i = 0; i < nbChild; ++i) // TODO compress recurss
		{
			GameObject* gameobject;
			_ar(childName, childTag, nbChild2);

			if (isPrefab)
				gameobject = prefabGameObjects.emplace(std::to_string((int)this + i), std::make_unique<GameObject>(childName, childTag)).first->second.get();
			else
				gameobject = CreateGameObject(childName, childTag);

			currentLoadedGameObject = gameobject;

			_ar(gameobject->components,
				gameobject->localPosition.x, gameobject->localPosition.y, gameobject->localPosition.z,
				gameobject->localRotation.x, gameobject->localRotation.y, gameobject->localRotation.z, gameobject->localRotation.w,
				gameobject->localScale.x, gameobject->localScale.y, gameobject->localScale.z, gameobject->isActive, gameobject->isPrefab);

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

			GameObject* gameobject;

			if (isPrefab)
				gameobject = prefabGameObjects.emplace(std::to_string((int)this + i), std::make_unique<GameObject>(childName, childTag)).first->second.get();
			else
				gameobject = CreateGameObject(childName, childTag);

			currentLoadedGameObject = gameobject;

			_ar(gameobject->components,
				gameobject->localPosition.x, gameobject->localPosition.y, gameobject->localPosition.z,
				gameobject->localRotation.x, gameobject->localRotation.y, gameobject->localRotation.z, gameobject->localRotation.w,
				gameobject->localScale.x, gameobject->localScale.y, gameobject->localScale.z, gameobject->isActive, gameobject->isPrefab);

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
			localScale.x, localScale.y, localScale.z, isActive, isPrefab);

		for (GameObject* child : children)
		{
			child->isPrefab = isPrefab;
			child->save(_ar);
		}
	}

};
