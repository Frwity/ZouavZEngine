#include "Game/Move.hpp"
#include "GameObject.hpp"
#include <iostream>
#include "Component/AudioBroadcaster.hpp"
#include "System/TimeManager.hpp"
#include "System/InputManager.hpp"

Move::Move(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void Move::Begin()
{}

void Move::Update()
{
	GetGameObject().localPosition = GetGameObject().localPosition + Vec3(sinf(TimeManager::GetTime()), 0.0f, cosf(TimeManager::GetTime()));
	GetGameObject().GetComponent<AudioBroadcaster>()->Update();

	if (InputManager::GetKeyPressedOneTime(E_KEYS::P))
		GetGameObject().GetComponent<AudioBroadcaster>()->Play();

	if (InputManager::GetKeyPressedOneTime(E_KEYS::K))
		GetGameObject().GetComponent<AudioBroadcaster>()->Stop();

	if (InputManager::GetKeyPressedOneTime(E_KEYS::U))
		GetGameObject().GetComponent<AudioBroadcaster>()->SetAmbient(true);

	if (InputManager::GetKeyPressedOneTime(E_KEYS::Y))
		GetGameObject().GetComponent<AudioBroadcaster>()->SetAmbient(false);

	if (InputManager::GetKeyPressedOneTime(E_KEYS::T))
		GetGameObject().GetComponent<AudioBroadcaster>()->SetLooping(true);

	if (InputManager::GetKeyPressedOneTime(E_KEYS::R))
		GetGameObject().GetComponent<AudioBroadcaster>()->SetLooping(false);
}

template <class Archive>
static void Move::load_and_construct(Archive& _ar, cereal::construct<Move>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
}