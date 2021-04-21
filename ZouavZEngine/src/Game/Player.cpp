#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
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

void Player::Begin()
{
	if (gameObject->GetComponent<Camera>())
	{
		gameObject->GetComponent<Camera>()->SetPosition({ 0, 5, -8 });
		gameObject->GetComponent<Camera>()->SetTarget({ 0, 0, 5 });

	}
	rb = gameObject->GetComponent<RigidBody>();
}

#include <iostream>

void Player::Update()
{
	Vec3 temp = gameObject->localRotation.ToEuler();
	temp.z += TimeManager::GetDeltaTime() * 6.0f;
	gameObject->localRotation = Quaternion(temp);

	if(!rb)
		rb = gameObject->GetComponent<RigidBody>();

	if (InputManager::GetKeyPressed(E_KEYS::RSHIFT))
		speed = 100;
	else
		speed = 3;
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
	//	rb->SetLinearVelocity(Vec3( 0.0f, 0.0f, -TimeManager::GetDeltaTime() * speed ));
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
	//	rb->SetLinearVelocity(Vec3( 0.0f, 0.0f, TimeManager::GetDeltaTime() * speed ));
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
	//	rb->SetLinearVelocity(Vec3( -TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f ));
	//if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
	//	rb->SetLinearVelocity(Vec3( TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f ));
	//if (InputManager::GetKeyPressed(E_KEYS::NUM0))
	//	rb->SetLinearVelocity(Vec3( 0.0f , TimeManager::GetDeltaTime() * speed, 0.0f ));


	if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
		gameObject->Translate({ 0.0f, 0.0f, -TimeManager::GetDeltaTime() * speed });
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
		gameObject->Translate({ 0.0f, 0.0f, TimeManager::GetDeltaTime() * speed });
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
		gameObject->Translate({ -TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f });
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
		gameObject->Translate({ TimeManager::GetDeltaTime() * speed, 0.0f, 0.0f });
	if (InputManager::GetKeyPressed(E_KEYS::NUM0))
		gameObject->Translate({ 0.0f , TimeManager::GetDeltaTime() * speed, 0.0f });
}
