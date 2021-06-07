#include "GameObject.hpp"
#include "Game/Arrow.hpp"
#include "Component/Animation.hpp"
#include "Game/EnemyArcher.hpp"

EnemyArcher::EnemyArcher(GameObject * _gameobject, std::string _name)
: Enemy(_gameobject, _name)
{
	detectionDistance = 50.0f;
	distanceToAttack = 15.0f;
	distanceToStop = 15.0f;
}

void EnemyArcher::Editor()
{
	Enemy::Editor();
	arrow.Editor("Arrow Prefab");
}

void EnemyArcher::Begin()
{
	attackAnimName = "Range Zombie Attack.fbx";
	walkAnimName = "Zombie Walk.fbx";
	idleAnimName = "Zombie Idle.fbx";
	deathAnimName = "Zombie_Death.fbx";
	Enemy::Begin();
}

void EnemyArcher::Update()
{
	Enemy::Update();
}

void EnemyArcher::Attack()
{
	if (*arrow)
	{
		Arrow* arrowComp = GameObject::Instanciate(*arrow, GetGameObject().WorldPosition())->GetComponent<Arrow>();
		if (arrowComp)
			arrowComp->Initiate(player->WorldPosition() - GetGameObject().WorldPosition(), attackDamage);
		timerAttackCooldown = attackCooldown;
		if (animation)
			animation->Play("Range Zombie Attack.fbx");
	}
}