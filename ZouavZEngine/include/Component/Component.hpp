#pragma once

class Component
{
private:
public:
	class GameObject* gameObject;

	Component(class GameObject* _gameObject);
	Component() = delete;
	virtual ~Component() = default;
};