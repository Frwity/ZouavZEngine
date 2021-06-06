#include "GameObject.hpp"
#include "System/TimeManager.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Component/BoxCollision.hpp"
#include "Component/Animation.hpp"
#include "Component/RigidBody.hpp"
#include "Game/Enemy.hpp"
#include "Game/EnemyManager.hpp"
#include <imgui.h>

Enemy::Enemy(GameObject * _gameobject, std::string _name)
: ICharacter(_gameobject, _name)
{
	if (_gameobject->IsActive())
		 EnemyManager::AddEnemy(this);
}

void Enemy::OnDeath()
{
	if (gameObject->IsActive())
		EnemyManager::RemoveEnemy(this);
}

void Enemy::Editor()
{
	ICharacter::Editor();
	ImGui::DragFloat("Detection Distance", &detectionDistance, 0.1f, 0.f);
	ImGui::DragFloat("Distance To Attack", &distanceToAttack, 0.1f, 0.f);
	ImGui::DragFloat("Distance To Stop", &distanceToStop, 0.1f, 0.f);
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f);
}

void Enemy::Begin()
{
	ICharacter::Begin();
	player = GameObject::GetGameObjectByTag("Player");
	distanceToAttack = 5.0f;
}

void Enemy::Update()
{
	if (!IsAlive())
	{
		decayTimer += TimeManager::GetDeltaTime();
		if (decayTimer > timeToDecay)
			GetGameObject().Destroy();
		return;
	}

	ICharacter::Update();

	if (player && !IsAttacking())
	{
		Vec3 direction = player->WorldPosition() - GetGameObject().WorldPosition();
		direction.y = 0;

		float distanceToPlayer = direction.GetMagnitude();

		Vec3 linearVelocity = rb->GetLinearVelocity();

		if (distanceToPlayer < detectionDistance)
		{
			float angleToPlayer = GetGameObject().Forward().SignedAngleToVector(direction, Vec3::up) + M_PI + M_PI;

			if (angleToPlayer > 0.2f)
				GetGameObject().RotateY((angleToPlayer / M_PI) * 180.0f);

			if (distanceToPlayer > distanceToStop)
			{
				rb->SetLinearVelocity(direction.Normalized() * TimeManager::GetDeltaTime() * speed * 100.0f + (linearVelocity.y * Vec3::up));
				PlayWalkAnimation();
			}

			if (distanceToPlayer < distanceToAttack && timerAttackCooldown >= attackCooldown && timerAttackDuration >= attackDuration && timerBeforeAttack >= beforeAttack)
				NeedToAttack();
		}

		else if (linearVelocity.x * linearVelocity.x > 0.1f || linearVelocity.z * linearVelocity.z > 0.1f)
			rb->SetLinearVelocity(linearVelocity.y * Vec3::up);
	}

	if (animation && !animation->IsPlaying())
		PlayIdleAnimation();
}
