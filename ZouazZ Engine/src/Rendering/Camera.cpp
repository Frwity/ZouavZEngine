#include "Rendering/Camera.hpp"
#include <cmath>

Camera::Camera(const Vec2& mouseStartPosition)
	: mousePosition(mouseStartPosition), position(0.0f, 0.0f, 0.0f), pitch(0.0f), yaw(0.0f)
{}

void Camera::MoveTo(const Vec3& direction)
{
	position += Vec3((cos(yaw) / 10) * direction.x + (sin(yaw) / 10) * direction.z, 0.0f, (sin(yaw) / 10) * direction.x + (cos(yaw) / 10) * direction.z);
}

Matrix4 Camera::GetMatrix()
{
	return Matrix4::CreateTRSMatrix(position, {pitch, yaw, 0}, { 1, 1, 1 }).Reverse();
}

void Camera::UpdateRotation(const Vec2& newMousePosition)
{
	float dx = mousePosition.x - newMousePosition.x;
	float dy = mousePosition.y - newMousePosition.y;

	yaw += dx / 1000;
	pitch += dy / 1000;

	mousePosition = newMousePosition;
}