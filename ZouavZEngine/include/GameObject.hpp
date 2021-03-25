#pragma once

#include "Component/Transform.hpp"
#include "Component/Component.hpp"
#include <vector>
#include <list>
#include <memory>

class GameObject : public Transform
{
	friend class Editor;
private:
	static std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<Component>> components;
	std::vector<GameObject*> children;
	GameObject* parent{ nullptr };

public:
	std::string name;
	std::vector<std::string> tags;

	GameObject() = delete;
	GameObject(const std::string& _name);
    ~GameObject() = default;

	static GameObject* CreateGameObject(const std::string& _name);

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

	static GameObject* GetGameObjectByName(std::string _name);
	static std::vector<GameObject*> GetGameObjectsByName(std::string _name);
	static GameObject* GetGameObjectByTag(std::string _tag);
	static std::vector<GameObject*> GetGameObjectsByTag(std::string _tag);
};