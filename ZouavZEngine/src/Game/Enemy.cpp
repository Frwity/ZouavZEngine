#include "GameObject.hpp"
#include "System/TimeManager.hpp"

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

		if ((direction).GetMagnitude() < detectionDistance)
			GetGameObject().Translate(direction.Normalized() * TimeManager::GetDeltaTime() * speed);
	}
}