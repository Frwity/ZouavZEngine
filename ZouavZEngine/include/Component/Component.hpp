#pragma once

class Component
{
private:
public:
	// TODO : not private
	class GameObject* gameObject;

	Component(class GameObject* _gameObject);
	Component() = delete;
	virtual ~Component() = default;

	template <class Archive>
	void save(Archive& _ar) const { _ar("oui"); }
};