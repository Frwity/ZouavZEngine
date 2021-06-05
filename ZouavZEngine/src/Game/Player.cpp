#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"
#include "GameObject.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/RigidBody.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/ProgressBar.hpp"
#include "Game/Enemy.hpp"
#include <imgui.h>
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
	ProgressBar* progressBar = GetGameObject().AddComponent<ProgressBar>();
	progressBar->currentValue = (float*)&life;
	progressBar->maxValue = (float*)&maxLife;
	progressBar->SetName("LifeBar");
	progressBar = GetGameObject().AddComponent<ProgressBar>();
	progressBar->currentValue = (float*)&currentXp;
	progressBar->maxValue = (float*)&maxXp;
	progressBar->SetName("XpBar");
	GetGameObject().SetTag("Player");
}

void Player::OnContact(Object* _other, class ShapeCollision* _triggerShape)
{
	if (!_other->GetTag().compare("Ground"))
	{
		isJumping = false;
	}
}

void Player::Begin()
{
	camera = GetGameObject().GetComponent<Camera>();
	camera->SetPosition({ 0.0f, 12.5f, -25.0f });
	camera->SetTarget({ 0.0f, 2.0f, 0.0f });
	ProgressBar* progressBar = GetGameObject().GetComponentByName<ProgressBar>("XpBar");
	progressBar->currentValue = (float*)&currentXp;
	progressBar->maxValue = (float*)&maxXp;
	progressBar->pos = { 0.0f, 1.0f, 0.0f };
	progressBar = GetGameObject().GetComponentByName<ProgressBar>("LifeBar");
	progressBar->currentValue = (float*)&life;
	progressBar->maxValue = (float*)&maxLife;
	progressBar->pos = { 0.0f, -1.0f, 0.0f };
	maxXp = level * 10;
	attackDamage += attackDamageGain * (level - 1);
	maxLife += lifeGain * (level - 1);
	ICharacter::Begin();
}

void Player::Update()
{
	if (!IsAlive())
		return;

	if (timerDashCooldown >= 0.0f)
		timerDashCooldown -= TimeManager::GetDeltaTime();

	if (timerDashDuration >= 0.0f)
	{
		timerDashDuration -= TimeManager::GetDeltaTime();

		if (timerDashDuration < 0.0f)
		{
			isDashing = false;
			timerDashCooldown = attackCooldown;
		}
	}

	ICharacter::Update();

	if (InputManager::GetKeyPressed(E_KEYS::RSHIFT))
		speed = 100;
	else
		speed = 3;

	Vec2 offset = InputManager::GetCursorOffsetFromLastFrame();

	Quaternion camRotY = Quaternion(Vec3{ 0.0f, -offset.x * camSensitivity, 0.0f });
	camera->SetPosition(camRotY.RotateVector(camera->GetPosition()));

	if (totalAngle < -M_PI_4 && offset.y > 0 || totalAngle > M_PI_4 && offset.y < 0)
		return;

	Vec3 right = (-camera->GetPosition().Cross(Vec3::up)).Normalized();
	Quaternion camRotX = Quaternion::AngleAxis(right, -offset.y * camSensitivity / 30.f);

	totalAngle += -offset.y * camSensitivity / 30.f;
	camera->SetPosition(camRotX.RotateVector((camera->GetPosition() - GetGameObject().WorldPosition()) + GetGameObject().WorldPosition()));

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

	if (InputManager::GetKeyPressedOneTime(E_KEYS::LCTRL))
	{
		isDashing = true;
		timerDashDuration = dashDuration;

		if (direction.GetSquaredMagnitude() > 0.01f)
			direction = GetGameObject().Forward();
	}

	if (isDashing)
		direction = direction.GetSquaredMagnitude() > 0.01f ? direction : dashDirection;

	Vec3 linearVelocity = rb->GetLinearVelocity();

	if (direction.GetSquaredMagnitude() > 0.01f)
	{
		float angleToDirection = GetGameObject().Forward().SignedAngleToVector(direction, Vec3::up) + M_PI + M_PI;

		GetGameObject().RotateY((angleToDirection / M_PI) * 180.0f);
	
		rb->SetLinearVelocity(GetGameObject().Forward() * TimeManager::GetDeltaTime() * (isDashing ? dashSpeed : speed) * 100.0f + (linearVelocity.y * Vec3::up));
	}

	else if (linearVelocity.x * linearVelocity.x > 0.1f || linearVelocity.z * linearVelocity.z > 0.1f)
		rb->SetLinearVelocity(linearVelocity.y * Vec3::up);

	if (InputManager::GetKeyPressed(E_KEYS::SPACEBAR) && !isJumping)
	{
		isJumping = true;
		rb->SetLinearVelocity(Vec3(0.0f, TimeManager::GetDeltaTime() * speed * 100.0f, 0.0f));
	}

	if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && timerAttackCooldown < 0.0f && timerAttackDuration < 0.0f)
		Attack();
}

void Player::ManageXp(const Enemy& _enemyKilled)
{
	currentXp += _enemyKilled.xpGiven;
	if (currentXp >= maxXp)
	{
		level++;
		currentXp -= maxXp;
		maxXp = level * 10;
		attackDamage += attackDamageGain;
		maxLife += lifeGain;
	}
}

void Player::Editor()
{
	ICharacter::Editor();
	ImGui::Text("Level : ");
	ImGui::SameLine();
	ImGui::DragInt("", &level);
	ImGui::Text("Max Xp : %d", maxXp);
	ImGui::Text("Current Xp : %d", currentXp);
}

void Player::PlayWalkAnimation()
{
	//if (animation)
	//	animation->Play("Player Walking.fbx");
}

void Player::PlayAttackAnimation()
{
	//if (animation)
	//	animation->Play("Player Attack.fbx");
}
void Player::PlayIdleAnimation()
{
	//if (animation)
	//	animation->Play("Player Idle.fbx");
}