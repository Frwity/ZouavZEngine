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
		//Play idle animation
		if (animComponent)
		{
			animComponent->loop = true;
			std::unordered_map<std::string, std::shared_ptr<AnimResource>>::iterator it = animComponent->animationsAttached.find("idle.fbx");
			if (it != animComponent->animationsAttached.end())
				animComponent->currentAnimation = it->second;
		}
	}
}

void Player::Begin()
{
	camera = GetGameObject().GetComponent<Camera>();
	camera->SetPosition({ 0.0f, 5.0f, -10.0f });
	camera->SetTarget({ 0.0f, 2.0f, 0.0f });
	ProgressBar* progressBar = GetGameObject().GetComponentByName<ProgressBar>("XpBar");
	progressBar->currentValue = (float*)&currentXp;
	progressBar->maxValue = (float*)&maxXp;
	progressBar = GetGameObject().GetComponentByName<ProgressBar>("LifeBar");
	progressBar->currentValue = (float*)&life;
	progressBar->maxValue = (float*)&maxLife;
	progressBar->pos = { 0.0f, 1.0f, 0.0f };
	maxXp = level * 10;
	attackDamage += attackDamageGain * (level - 1);
	maxLife += lifeGain * (level - 1);
	ICharacter::Begin();
	animComponent = gameObject->GetComponent<Animation>();

	if (!animComponent)
		Debug::LogWarning("Anim component not found");
	else
	{
		std::unordered_map<std::string, std::shared_ptr<AnimResource>>::iterator it = animComponent->animationsAttached.find("idle.fbx");
		if (it != animComponent->animationsAttached.end())
			animComponent->currentAnimation = it->second;
		animComponent->Play();
	}


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

		//Play jump animation
		if (animComponent)
		{
			std::unordered_map<std::string, std::shared_ptr<AnimResource>>::iterator it = animComponent->animationsAttached.find("jumping_up.fbx");
			if (it != animComponent->animationsAttached.end())
				animComponent->currentAnimation = it->second;
			animComponent->loop = false;
		}
	}

	if (InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON::BUTTON_LEFT) && timerAttackCooldown < 0.0f && timerAttackDuration < 0.0f)
		Attack();

	Vec2 offset = InputManager::GetCursorOffsetFromLastFrame();

	Quaternion camRot = Quaternion(Vec3{ 0.0f, -offset.x * camSensitivity, 0.0f }) * Quaternion(Vec3{ -offset.y * camSensitivity, 0.0f, 0.0f });

	camera->SetPosition(camRot.RotateVector(camera->GetPosition()));
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