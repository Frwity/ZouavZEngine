#pragma once
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Component/Component.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

enum class E_LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT
};

class Light : public Component
{
public:
	Vec3 ambient{ 0.5f, 0.5f, 0.5f };
	Vec3 diffuse{ 0.5f, 0.5f, 0.5f };
	Vec3 specular{ 0.5f, 0.5f, 0.5f };
	Vec3 constLineQuad{ 1.0f, 0.01f, 0.001f };
	Vec3 direction{ 0.0f, -1.0f, 0.0f };
	Vec2 cutOffOuterCutOff{ 0.9f, 0.8f };
	E_LIGHT_TYPE type{ E_LIGHT_TYPE::DIRECTIONAL };

	Light() = delete;
	Light(class GameObject* _gameObject);
	Light(class GameObject* _gameObject, const Vec3 _ambient, const Vec3 _diffuse, const Vec3 _specular, const Vec3 _constLineQuad, const Vec3 _direction, const Vec2 _cutOffOuterCutOff, E_LIGHT_TYPE _type);
	Light(const Light&) = default;
	Component* Clone() const override { return new Light(*this); }
	~Light();

	bool toDestroy{ false };

	void Editor() override;

	const char* GetComponentName() override { return "Light"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(ambient.x, ambient.y, ambient.z,
			diffuse.x, diffuse.y, diffuse.z,
			specular.x, specular.y, specular.z,
			constLineQuad.x, constLineQuad.y, constLineQuad.z,
			direction.x, direction.y, direction.z,
			cutOffOuterCutOff.x, cutOffOuterCutOff.y,
			(int)type);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Light>& _construct);
};


CEREAL_REGISTER_TYPE(Light)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Light)
