#include "Rendering/Material.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/FontComponent.hpp"

#include "System/TimeManager.hpp"

#include "GameObject.hpp"
#include "Game/ICharacter.hpp"

#include "imgui.h"

ICharacter::ICharacter(GameObject* _gameobject)
	: ScriptComponent(_gameobject)
{
}

void ICharacter::Editor()
{
	ImGui::ColorEdit4("Damage Color : ", &damageColor.x);
	ImGui::DragInt("Life : ", &life);
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
}

void ICharacter::Update()
{
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

	life -= _damage;

	if (IsAlive())
	{
		asTakenDamage = true;
		if (lifeFont)
			lifeFont->ChangeText(std::to_string(life));
	}
	else
		return false;

	return true;
}