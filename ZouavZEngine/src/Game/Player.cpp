#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
#include "GameObject.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/RigidBody.hpp"
#include "Component/BoxCollision.hpp"
#include "Game/Player.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Player::Player(GameObject* _gameobject, std::string _name)
	: ICharacter(_gameobject, _name)
{

}

void Player::OnAddComponent()
{
	ICharacter::OnAddComponent();
	camera = GetGameObject().AddComponent<Camera>();
	GetGameObject().SetTag("Player");
}

void Player::OnContact(Object* _other, class ShapeCollision* _triggerShape)
{
	if (!_other->GetTag().compare("Ground"))
		isJumping = false;
}

void Player::Begin()
{
	ICharacter::Begin();
	camera = GetGameObject().GetComponent<Camera>();
	camera->SetPosition({ 0.0f, 5.0f, -10.0f });
	camera->SetTarget({ 0.0f, 2.0f, 0.0f });
	life = 10000;
}

void Player::Update()
{
	if (!IsAlive())
		return;

	ICharacter::Update();

	if (InputManager::GetKeyPressed(E_KEYS::RSHIFT))
		speed = 100;
	else
		speed = 3;

	Vec3 cameraForward = camera->GetTarget() - camera->GetPosition();
	cameraForward.y = camera->GetPosition().y;
	cameraForward.Normalize();
	Vec3 cameraRight = cameraForward.Cross(Vec3::up).Normalized();

	Vec3 direction;

	if (InputManager::GetKeyPressed(E_KEYS::ARROW_UP))
		direction += cameraForward;
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_DOWN))
		direction -= cameraForward;
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_RIGHT))
		direction += cameraRight;
	if (InputManager::GetKeyPressed(E_KEYS::ARROW_LEFT))
		direction -= cameraRight;

	if (direction.GetSquaredMagnitude() > 0.01f)
	{
		float angleToDirection = GetGameObject().Forward().SignedAngleToVector(direction, Vec3::up) + M_PI + M_PI;

		GetGameObject().RotateY((angleToDirection / M_PI) * 180.0f);
	
		GetGameObject().Translate(GetGameObject().Forward() * TimeManager::GetDeltaTime() * speed);
	}

	if (InputManager::GetKeyPressed(E_KEYS::SPACEBAR) && !isJumping)
	{
		isJumping = true;
		rb->SetLinearVelocity(Vec3(0.0f, TimeManager::GetDeltaTime() * speed * 100.0f, 0.0f));
	}

	if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && timerAttackCooldown < 0.0f && timerAttackDuration < 0.0f)
		Attack();

	Vec2 offset = InputManager::GetCursorOffsetFromLastFrame();

	Quaternion camRot = Quaternion(Vec3{ 0.0f, -offset.x * camSensitivity, 0.0f }) * Quaternion(Vec3{ -offset.y * camSensitivity, 0.0f, 0.0f });

	camera->SetPosition(camRot.RotateVector(camera->GetPosition()));
}
