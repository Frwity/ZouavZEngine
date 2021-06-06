#include "GameObject.hpp"
#include "Component/Animation.hpp"
#include "Game/Zombie.hpp"

Zombie::Zombie(GameObject * _gameobject, std::string _name)
: Enemy(_gameobject, _name)
{}

void Zombie::Editor()
{
   Enemy::Editor();
}

void Zombie::Begin()
{
   Enemy::Begin();
   attackAnim = "Zombie Punching.fbx";
   walkAnim = "Zombie Walk.fbx";
}

void Zombie::Update()
{
   Enemy::Update();
}

void Zombie::PlayWalkAnimation()
{
	if (animation && !animation->IsPlaying("Zombie Walk.fbx"))
		animation->Play("Zombie Walk.fbx");
}

void Zombie::PlayAttackAnimation()
{
	if (animation)
		animation->Play("Zombie Punching.fbx");
}
void Zombie::PlayIdleAnimation()
{
	if (animation)
		animation->Play("Zombie Idle.fbx");
}

bool Zombie::CanAttack()
{
	return animation->IsFinish("Zombie Punching.fbx") || !animation->IsPlaying("Zombie Punching.fbx");
}