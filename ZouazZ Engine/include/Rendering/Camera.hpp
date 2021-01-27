#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Matrix4.hpp"

class Camera
{
	Vec2 mousePosition;
	Vec3 position;
	float pitch, yaw;

public:
	Camera() {};
	Camera(const Vec2& mouseStartPosition);
	~Camera() = default;

	void MoveTo(const Vec3& direction);
	Matrix4 GetMatrix();
	void UpdateRotation(const Vec2& newMousePosition);
};