#include "Component/Light.hpp"

Light::Light(class GameObject* _gameObject, const Vec3 _ambient, const Vec3 _diffuse, const Vec3 _specular, const Vec3 _constLineQuad, const Vec3 _direction, const Vec2 _cutOffOuterCutOff, E_LIGHT_TYPE _type)
	: Component(_gameObject), ambient(_ambient), diffuse(_diffuse), specular(_specular), constLineQuad(_constLineQuad), direction(_direction), cutOffOuterCutOff(_cutOffOuterCutOff), type(_type)
{

}