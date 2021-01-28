#pragma once

#include "Component/Transform.hpp"
#include "Component/Component.hpp"
#include <vector>
#include <memory>

class GameObject
{
private:
    std::vector<std::unique_ptr<Component>> components;

public:
    Transform transform;

    GameObject();
    ~GameObject() = default;

    void AddChild(GameObject& _child);

    
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