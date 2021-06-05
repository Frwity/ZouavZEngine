#include "GameObject.hpp"
#include "System/TimeManager.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Component/BoxCollision.hpp"
#include "Component/Animation.hpp"
#include "Component/RigidBody.hpp"
#include "Game/Enemy.hpp"
#include "Game/EnemyManager.hpp"

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
}

void Enemy::Begin()
{
	ICharacter::Begin();
	player = GameObject::GetGameObjectByTag("Player");
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

			if (distanceToPlayer < distanceToAttack && timerAttackCooldown < 0.0f && timerAttackDuration < 0.0f)
				Attack();
		}

		else if (linearVelocity.x * linearVelocity.x > 0.1f || linearVelocity.z * linearVelocity.z > 0.1f)
			rb->SetLinearVelocity(linearVelocity.y * Vec3::up);
	}

	if (animation && !animation->IsPlaying())
		PlayIdleAnimation();
}

void Enemy::PlayWalkAnimation()
{
	if (animation)
		animation->Play("Zombie Walk.fbx");
}

void Enemy::PlayAttackAnimation()
{
	if (animation)
		animation->Play("Zombie Punching.fbx");
}
void Enemy::PlayIdleAnimation()
{
	if (animation)
		animation->Play("Zombie Idle.fbx");
}