#include "Game/Move.hpp"
#include "GameObject.hpp"
#include <iostream>
#include "Component/AudioBroadcaster.hpp"
#include "System/TimeManager.hpp"
#include "System/InputManager.hpp"

Move::Move(GameObject * _gameobject)
: ScriptComponent(_gameobject)
{}

void Move::Begin()
{}

void Move::Update()
{
	gameObject->position = gameObject->position + Vec3::Forward() * TimeManager::GetDeltaTime() * 5.0f;//Vec3(sinf(TimeManager::GetTime()) * 100.0f, 0.0f, cosf(TimeManager::GetTime()) * 100.0f);
	gameObject->GetComponent<AudioBroadcaster>()->Update();

	std::cout << gameObject->position.z << std::endl;

	if (InputManager::GetKeyPressedOneTime(E_KEYS::P))
		gameObject->GetComponent<AudioBroadcaster>()->Play();

	if (InputManager::GetKeyPressedOneTime(E_KEYS::K))
		gameObject->GetComponent<AudioBroadcaster>()->Stop();

	if (InputManager::GetKeyPressedOneTime(E_KEYS::U))
		gameObject->GetComponent<AudioBroadcaster>()->SetAmbient(true);

	if (InputManager::GetKeyPressedOneTime(E_KEYS::Y))
		gameObject->GetComponent<AudioBroadcaster>()->SetAmbient(false);

	if (InputManager::GetKeyPressedOneTime(E_KEYS::T))
		gameObject->GetComponent<AudioBroadcaster>()->SetLooping(true);

	if (InputManager::GetKeyPressedOneTime(E_KEYS::R))
		gameObject->GetComponent<AudioBroadcaster>()->SetLooping(false);
}