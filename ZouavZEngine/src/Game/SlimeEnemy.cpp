#include "GameObject.hpp"
#include "Game/SlimeEnemy.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/RigidBody.hpp"
#include "System/TimeManager.hpp"

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
	Enemy::OnDeath();
	if (++dividedCount > maxDivision)
		return;
	GetGameObject().GetComponent<BoxCollision>()->Deactivate();
	GetGameObject().GetComponent<RigidBody>()->Deactivate();

	float spid = TimeManager::GetDeltaTime() * speed * 85.f;
	Vec3 pos = GetGameObject().WorldPosition();
	GameObject* deathSlime = GameObject::Instanciate(&GetGameObject(), pos + 1.f);
	deathSlime->GetComponent<RigidBody>()->SetLinearVelocity(Vec3(spid, TimeManager::GetDeltaTime() * speed * 100.f, spid));
	deathSlime->GetComponent<SlimeEnemy>()->maxLife = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->life = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->dividedCount = dividedCount;
	deathSlime->MultiplyScaleBy({0.66f, 0.66f, 0.66f});

	deathSlime = GameObject::Instanciate(&GetGameObject(), pos + Vec3{ -1.f, 1.f, -1.f });
	deathSlime->GetComponent<RigidBody>()->SetLinearVelocity(Vec3(-spid, TimeManager::GetDeltaTime() * speed * 100.f, -spid));
	deathSlime->GetComponent<SlimeEnemy>()->maxLife = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->life = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->dividedCount = dividedCount;
	deathSlime->MultiplyScaleBy({ 0.66f, 0.66f, 0.66f });
}