#pragma once
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Component/Component.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

enum class E_LIGHT_TYPE
{
	Directional,
	Point,
	Spot
};

class Light : public Component
{
public:
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 constLineQuad;
	Vec3 direction;
	Vec2 cutOffOuterCutOff;
	E_LIGHT_TYPE type;

	Light() = delete;
	Light(class GameObject* _gameObject, const Vec3 _ambient, const Vec3 _diffuse, const Vec3 _specular, const Vec3 _constLineQuad, const Vec3 _direction, const Vec2 _cutOffOuterCutOff, E_LIGHT_TYPE _type);

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
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Light>& _construct)
	{
		Vec3 ambient;
		Vec3 diffuse;
		Vec3 specular;
		Vec3 constLineQuad;
		Vec3 direction;
		Vec2 cutOffOuterCutOff;
		int type;

		_ar(ambient.x, ambient.y, ambient.z,
			diffuse.x, diffuse.y, diffuse.z,
			specular.x, specular.y, specular.z,
			constLineQuad.x, constLineQuad.y, constLineQuad.z,
			direction.x, direction.y, direction.z,
			cutOffOuterCutOff.x, cutOffOuterCutOff.y,
			type);

		_construct(nullptr, ambient, diffuse, specular, constLineQuad, direction, cutOffOuterCutOff, (E_LIGHT_TYPE)type);
	}
};


CEREAL_REGISTER_TYPE(Light)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Light)