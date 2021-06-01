#include "Rendering/Material.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/FontComponent.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/RigidBody.hpp"
#include "Component/AudioBroadcaster.hpp"

#include "System/TimeManager.hpp"

#include "GameObject.hpp"
#include "imgui.h"

#include "Game/Enemy.hpp"
#include "Game/Player.hpp"
#include "Game/ICharacter.hpp"

ICharacter::ICharacter(GameObject* _gameobject, std::string _name)
	: ScriptComponent(_gameobject, _name)
{
}

void ICharacter::OnAddComponent()
{
	GetGameObject().AddComponent<BoxCollision>();
	rb = GetGameObject().AddComponent<RigidBody>();
	rb->lockX = true;
	rb->lockY = true;
	rb->lockZ = true;
	rb->LockAxis();
	material = &GetGameObject().AddComponent<MeshRenderer>()->material;
	attackCollision = GetGameObject().AddComponent<BoxCollision>();
	attackCollision->SetName("Attack Collision");
	attackCollision->SetTrigger(true);
	attackCollision->Deactivate();
	attackCollision->halfExtends.z = 1.5f;
	attackCollision->EditPosition({ 0.0f, 0.0f, 1.25f });
	baseColor = material->color;
	GetGameObject().AddComponent<FontComponent>();
	audioBroadcaster = GetGameObject().AddComponent<AudioBroadcaster>();
}

void ICharacter::OnTrigger(Object* _other, ShapeCollision* _triggerShape)
{
	GameObject* go = dynamic_cast<GameObject*>(_other);
	if (!go)
		return;
	if (_triggerShape == attackCollision)
	{
		Enemy* otherEnemy = go->GetComponent<Enemy>();
		Enemy* thisEnemy = GetGameObject().GetComponent<Enemy>();
		if ((otherEnemy && !thisEnemy) || (!otherEnemy && thisEnemy))
		{
			if (go->GetComponent<ICharacter>()->IsAlive())
			{
				go->GetComponent<ICharacter>()->Damage(attackDamage);
				if (otherEnemy && !otherEnemy->IsAlive())
					GetGameObject().GetComponent<Player>()->ManageXp(*otherEnemy);
			}
		}
	}
}

void ICharacter::Editor()
{
	ImGui::ColorEdit4("Damage Color : ", &damageColor.x);
	ImGui::DragInt("Life : ", &life, 1.0f, 0, maxLife);
	ImGui::Text("Max Life : %d", maxLife);
}

void ICharacter::Begin()
{
	if (GetGameObject().GetComponent<MeshRenderer>())
	{
		material = &GetGameObject().GetComponent<MeshRenderer>()->material;
		baseColor = material->color;
	}
	if (GetGameObject().GetComponent<FontComponent>())
	{
		lifeFont = GetGameObject().GetComponent<FontComponent>();
		lifeFont->ChangeText(std::to_string(life));
	}

	attackCollision = GetGameObject().GetComponentByName<BoxCollision>("Attack Collision");
	rb = GetGameObject().GetComponent<RigidBody>();
	audioBroadcaster = GetGameObject().GetComponent<AudioBroadcaster>();
	life > maxLife ? life = maxLife : 0;
}

void ICharacter::Update()
{
	if (!IsAlive())
		return;

	if (timerAttackCooldown >= 0.0f)
		timerAttackCooldown -= TimeManager::GetDeltaTime();

	if (timerAttackDuration >= 0.0f)
	{
		timerAttackDuration -= TimeManager::GetDeltaTime();

		if (timerAttackDuration < 0.0f)
		{
			attackCollision->Deactivate();
			timerAttackCooldown = attackCooldown;
		}
	}

	if (asTakenDamage)
	{
		if (material)
			material->color = Vec4::Lerp(damageColor, baseColor, invulnerabilityTimer / invulnerabilityFrame);
		invulnerabilityTimer += TimeManager::GetDeltaTime();
		if (invulnerabilityTimer >= invulnerabilityFrame)
		{
			asTakenDamage = false;
			invulnerabilityTimer = 0.0f;
			if (material)
				material->color = baseColor;
		}
	}
}

bool ICharacter::Damage(int _damage)
{
	if (asTakenDamage)
		return false;

	if (IsAlive())
	{
		life -= _damage;
		if (IsAlive())
		{
			asTakenDamage = true;
			if (lifeFont)
				lifeFont->ChangeText(std::to_string(life));
			audioBroadcaster->Play("damage.wav");
		}
		else
		{
			audioBroadcaster->Play("death.wav");
			attackCollision->Deactivate();
		}
	}
	else
		return false;

	return true;
}

void ICharacter::Attack()
{
	attackCollision->Activate();
	timerAttackDuration = attackDuration;
	audioBroadcaster->Play("attack.wav");
}