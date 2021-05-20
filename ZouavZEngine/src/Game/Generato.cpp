#include "Game/Generato.hpp"
#include "System/InputManager.hpp"
#include "GameObject.hpp"

Generato::Generato(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void Generato::Begin()
{

}

void Generato::Update()
{
	if (InputManager::GetKeyPressed(E_KEYS::T))
	{
		GameObject::Instanciate(*prefab, gameObject->WorldPosition());
	}
}

template <class Archive>
void Generato::load_and_construct(Archive& _ar, cereal::construct<Generato>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(cereal::base_class<Component>(_construct.ptr()));
}

void Generato::Editor()
{
	prefab.Editor("Prefab");
}