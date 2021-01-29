#pragma once
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"

enum class E_LIGHT_TYPE
{
	Directional,
	Point,
	Spot
};

class Light
{
protected:
public:
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 position;
	Vec3 constLineQuad;
	Vec3 direction;
	Vec2 cutOffOuterCutOff;
	E_LIGHT_TYPE type;

	Light(const Vec3 _ambient, const Vec3 _diffuse, const Vec3 _specular, Vec3 _position, const Vec3 _constLineQuad, const Vec3 _direction, const Vec2 _cutOffOuterCutOff, E_LIGHT_TYPE _type);
};