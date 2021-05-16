#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
#include "System/ScriptSystem.hpp"
#include "GameObject.hpp"
#include "Rendering/Camera.hpp"
#include "Component/RigidBody.hpp"
#include "Game/Player.hpp"


#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Player::Player(GameObject* _gameobject)
	: ScriptComponent(_gameobject)
{

}

void Player::Editor()
{
	skull.Editor("skull");
}

void Player::Begin()
{
	if (GetGameObject().GetComponent<Camera>())
	{
		GetGameObject().GetComponent<Camera>()->SetPosition({ 0, 5, 8 });
		GetGameObject().GetComponent<Camera>()->SetTarget({ 0, 0, -5 });

	}
	rb = GetGameObject().GetComponent<RigidBody>();
}

#include <iostream>

void Player::Update()
{
	if (!rb)
		rb = GetGameObject().GetComponent<RigidBody>();

	if (InputManager::GetKeyPressed(E_KEYS::RSHIFT))
		speed = 100;
	else
		speed = 3;

	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
	//	rb->SetLinearVelocity(Vec3(0.0f, 0.0f, -TimeManager::GetDeltaTime() * speed));
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
	//	rb->SetLinearVelocity(Vec3(0.0f, 0.0f, TimeManager::GetDeltaTime() * speed));
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
	//	rb->SetLinearVelocity(Vec3(-TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f));
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
	//	rb->SetLinearVelocity(Vec3(TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f));
	//if (InputManager::GetKeyPressed(E_KEYS::NUM0))
	//	rb->SetLinearVelocity(Vec3(0.0f, TimeManager::GetDeltaTime() * speed, 0.0f));
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
		GetGameObject().Translate({ 0.0f, 0.0f, -TimeManager::GetDeltaTime() * speed });
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
		GetGameObject().Translate({ 0.0f, 0.0f, TimeManager::GetDeltaTime() * speed });
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
		GetGameObject().Translate({ -TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f });
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
		GetGameObject().Translate({ TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f });
	if (InputManager::GetKeyPressed(E_KEYS::NUM0))
		GetGameObject().Translate({ 0.0f , TimeManager::GetDeltaTime() * speed, 0.0f });

	if (InputManager::GetKeyPressed(E_KEYS::T))
		GameObject::Instanciate(*skull, gameObject->WorldPosition());
}
