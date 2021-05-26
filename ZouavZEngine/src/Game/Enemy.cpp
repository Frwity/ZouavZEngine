#include "GameObject.hpp"
#include "System/TimeManager.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#include "Game/Enemy.hpp"

Enemy::Enemy(GameObject * _gameobject)
: ICharacter(_gameobject)
{}

void Enemy::Editor()
{}

void Enemy::Begin()
{
	player = GameObject::GetGameObjectByTag("Player");
}

void Enemy::Update()
{
	if (player)
	{
		Vec3 direction = player->WorldPosition() - GetGameObject().WorldPosition();

		float angleToPlayer = GetGameObject().Forward().SignedAngleToVector(direction, Vec3::up) + M_PI + M_PI;

		if (angleToPlayer > 0.2f)
			GetGameObject().RotateY((angleToPlayer / M_PI) * 180.0f);

		if ((direction).GetMagnitude() < detectionDistance)
			GetGameObject().Translate(direction.Normalized() * TimeManager::GetDeltaTime() * speed);
	}
}