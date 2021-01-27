#pragma once

class Component
{
private:
public:
	class GameObject* gameObject{nullptr};

	Component(class GameObject* _gameObject);
	Component() = default;
	virtual ~Component() = default;
};