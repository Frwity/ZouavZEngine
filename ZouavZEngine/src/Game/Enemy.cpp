#include "GameObject.hpp"
#include "System/TimeManager.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Component/BoxCollision.hpp"

#include "Game/Enemy.hpp"

Enemy::Enemy(GameObject * _gameobject, std::string _name)
: ICharacter(_gameobject, _name)
{}

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

	if (player)
	{
		Vec3 direction = player->WorldPosition() - GetGameObject().WorldPosition();

		float angleToPlayer = GetGameObject().Forward().SignedAngleToVector(direction, Vec3::up) + M_PI + M_PI;

		if (angleToPlayer > 0.2f)
			GetGameObject().RotateY((angleToPlayer / M_PI) * 180.0f);

		float distanceToPlayer = direction.GetMagnitude();

		if (distanceToPlayer < detectionDistance)
		{
			if (distanceToPlayer > distanceToStop)
				GetGameObject().Translate(direction.Normalized() * TimeManager::GetDeltaTime() * speed);

			if (distanceToPlayer < distanceToAttack && timerAttackCooldown < 0.0f && timerAttackDuration < 0.0f)
				Attack();
		}
	}
}