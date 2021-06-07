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
	attackAnimName = "Slime Attack.fbx";
	walkAnimName = "Slime Walk.fbx";
	idleAnimName = "Slime_Idle.fbx";
	deathAnimName = "Slime Death.fbx";
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

	float spid = TimeManager::GetDeltaTime() * speed * 85.f;
	Vec3 pos = GetGameObject().WorldPosition() + Vec3::right * 3.0f + Vec3::up * 2.0f;
	GameObject* deathSlime = GameObject::Instanciate(&GetGameObject(), pos + 1.f);
	deathSlime->GetComponent<RigidBody>()->SetLinearVelocity(Vec3(spid, TimeManager::GetDeltaTime() * speed * 100.f, spid));
	deathSlime->GetComponent<BoxCollision>()->EditPosition(deathSlime->GetComponent<BoxCollision>()->GetPosition() - Vec3::up * 0.2f);
	deathSlime->GetComponent<SlimeEnemy>()->maxLife = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->life = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->dividedCount = dividedCount;
	deathSlime->GetComponent<SlimeEnemy>()->invulnerabilityTimer = 0.0f;
	deathSlime->MultiplyScaleBy({0.66f, 0.66f, 0.66f});

	pos = GetGameObject().WorldPosition() + Vec3::right * -3.0f + Vec3::up * 2.0f;
	deathSlime = GameObject::Instanciate(&GetGameObject(), pos + Vec3{ -1.f, 1.f, -1.f });
	deathSlime->GetComponent<RigidBody>()->SetLinearVelocity(Vec3(-spid, TimeManager::GetDeltaTime() * speed * 100.f, -spid));
	deathSlime->GetComponent<BoxCollision>()->EditPosition(deathSlime->GetComponent<BoxCollision>()->GetPosition() - Vec3::up * 0.33f);
	deathSlime->GetComponent<SlimeEnemy>()->maxLife = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->life = maxLife / 2;
	deathSlime->GetComponent<SlimeEnemy>()->dividedCount = dividedCount;
	deathSlime->GetComponent<SlimeEnemy>()->invulnerabilityTimer = 0.0f;
	deathSlime->MultiplyScaleBy({ 0.66f, 0.66f, 0.66f });
}