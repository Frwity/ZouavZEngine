#pragma once

#include "Component/Transform.hpp"
#include "Component/Component.hpp"
#include <vector>
#include <list>
#include <memory>

class GameObject : public Transform
{
private:
    std::vector<std::unique_ptr<Component>> components;
	std::vector<GameObject*> children;
	GameObject* parent{ nullptr };

public:
    
	GameObject() = delete;
	GameObject(GameObject* _parent);
    ~GameObject() = default;

	static GameObject* CreateGameObject();

	const std::vector<GameObject*>& GetChildren() const { return children; }

	void AddChild(GameObject* _child)
	{
		children.push_back(_child);
		if (children.back()->parent)
			children.back()->parent->RemoveChild(children.back());
		children.back()->parent = this;
	}

	const GameObject* GetParent() const { return parent; }

	void SetParent(GameObject* _parent) 
	{ 
		if (parent)
			parent->RemoveChild(this);

		parent = _parent;

		if (_parent)
			_parent->children.push_back(this);
	}

	void RemoveChild(GameObject* _child)
	{
		for (auto it = children.begin(); it != children.end(); ) 
		{
			if (*it == _child) 
				it = children.erase(it);
			else
				++it;
		}
	}

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
};