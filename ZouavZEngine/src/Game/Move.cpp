#include "Game/Move.hpp"
#include "GameObject.hpp"
#include <iostream>
#include "Component/AudioBroadcaster.hpp"
#include "System/TimeManager.hpp"

Move::Move(GameObject * _gameobject)
: ScriptComponent(_gameobject)
{}

void Move::Begin()
{}

void Move::Update()
{
	gameObject->position = gameObject->position + Vec3(sinf(TimeManager::GetTime()) / 10.0f, 0.0f, cosf(TimeManager::GetTime()) / 10.0f);
	gameObject->GetComponent<AudioBroadcaster>()->Update();
}