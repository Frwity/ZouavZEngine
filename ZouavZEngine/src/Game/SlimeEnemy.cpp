#include "GameObject.hpp"
#include "Game/SlimeEnemy.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/RigidBody.hpp"

SlimeEnemy::SlimeEnemy(GameObject * _gameobject, std::string _name)
: Enemy(_gameobject, _name)
{}

void SlimeEnemy::Editor()
{
	Enemy::Editor();
}

void SlimeEnemy::Begin()
{
	Enemy::Begin();
}

void SlimeEnemy::Update() 
{
	Enemy::Update();

	if (!IsAlive())
		return;
}

void SlimeEnemy::OnDeath()
{
	if (++dividedCount > maxDivision)
		return;
	GetGameObject().GetComponent<BoxCollision>()->Deactivate();
	GetGameObject().GetComponent<RigidBody>()->Deactivate();

	GameObject* deathSlime = GameObject::Instanciate(&GetGameObject());
	deathSlime->GetComponent<SlimeEnemy>()->maxLife = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->life = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->dividedCount = dividedCount;
	deathSlime->MultiplyScaleBy({0.66f, 0.66f, 0.66f});

	deathSlime = GameObject::Instanciate(&GetGameObject());
	deathSlime->GetComponent<SlimeEnemy>()->maxLife = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->life = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->dividedCount = dividedCount;
	deathSlime->MultiplyScaleBy({ 0.66f, 0.66f, 0.66f });
}