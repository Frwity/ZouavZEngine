#include "Rendering/Light.hpp"

Light::Light(const Vec3 _ambient, const Vec3 _diffuse, const Vec3 _specular, Vec3 _position, const Vec3 _constLineQuad, const Vec3 _direction, const Vec2 _cutOffOuterCutOff, E_LIGHT_TYPE _type) :
	ambient(_ambient),
	diffuse(_diffuse),
	specular(_specular),
	position(_position),
	constLineQuad(_constLineQuad),
	direction(_direction),
	cutOffOuterCutOff(_cutOffOuterCutOff),
	type(_type)
{

}