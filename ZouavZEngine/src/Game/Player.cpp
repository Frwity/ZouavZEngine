#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
#include "GameObject.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/RigidBody.hpp"
#include "Component/BoxCollision.hpp"
#include "Game/Player.hpp"
#include "Game/ThirdPersonCamera.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Player::Player(GameObject* _gameobject, std::string _name)
	: ICharacter(_gameobject, _name)
{

}

void Player::OnAddComponent()
{
	playerMesh = GameObject::CreateGameObject("PlayerMesh");
	playerMesh->SetParent(&GetGameObject());
	playerMesh->AddComponent<MeshRenderer>();
	GameObject* camera = GameObject::CreateGameObject("Camera");
	camera->SetParent(&GetGameObject());
	camera->AddComponent<ThirdPersonCamera>();
	GetGameObject().AddComponent<BoxCollision>();
	rb = GetGameObject().AddComponent<RigidBody>();
	attackCollision = GetGameObject().AddComponent<BoxCollision>();
	attackCollision->SetName("Attack Collision");
	attackCollision->SetTrigger(true);
	attackCollision->Dehactivate();
}

void Player::OnTrigger(GameObject* _other, ShapeCollision* _triggerShape)
{
	if (_triggerShape == attackCollision)
	{
		ICharacter* characterOther = _other->GetComponent<ICharacter>();
		if (characterOther)
			characterOther->Damage(attackDamage);
	}
}

void Player::Begin()
{
	playerMesh = &GetGameObject().GetChild(0);
	material = &playerMesh->GetComponent<MeshRenderer>()->material;
	baseColor = material->color;

	ICharacter::Begin();

	rb = GetGameObject().GetComponent<RigidBody>();
	attackCollision = GetGameObject().GetComponentByName<BoxCollision>("Attack Collision");
}

void Player::Update()
{
	ICharacter::Update();

	if (timerAttackCooldown >= 0.0f)
		timerAttackCooldown -= TimeManager::GetDeltaTime();

	if (timerAttackDuration >= 0.0f)
	{
		timerAttackDuration -= TimeManager::GetDeltaTime();

		if (timerAttackDuration < 0.0f)
		{
			attackCollision->Dehactivate();
			timerAttackCooldown = attackCooldown;
		}
	}

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
		Damage(1);

	if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && timerAttackCooldown < 0.0f && timerAttackDuration < 0.0f)
	{
		attackCollision->Activate();
		timerAttackDuration = attackDuration;
	}
}
