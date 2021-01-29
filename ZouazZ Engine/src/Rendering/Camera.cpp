#include "Rendering/Camera.hpp"
#include "Maths/Mat4.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Camera::Camera(const Vec2& mouseStartPosition, int width, int height)
	: mousePosition(mouseStartPosition), position(0.0f, 0.0f, 0.0f), pitch(0.0f), yaw(0.0f), speed{30.0f}
{
    projection = Mat4::CreatePerspectiveProjectionMatrix(width, height, 0.01, 1000, 45); 
}

float Camera::Speed() const
{
	return speed;
}

void Camera::MoveTo(const Vec3& direction)
{
	position += Vec3((sin(yaw) / 10) * direction.z + (cos(yaw) / 10) * direction.x, direction.y / 10 - (sin(pitch) / 10) * direction.z, (cos(yaw) / 10) * direction.z - (sin(yaw) / 10) * direction.x);
}

Mat4 Camera::GetMatrix() const
{
    return Mat4::CreateTRSMatrix(position, { pitch, yaw, 0 }, { 1, 1, 1 });
}

Mat4 Camera::GetLookAtMatrix(const Vec3& target) const
{
    const Vec3 forward = (target - position).Normalized();
    const Vec3 right = (forward.Cross(Vec3::Up())).Normalized();
    const Vec3 up = right.Cross(forward);

    Mat4 cameraMatrix;

    cameraMatrix.Accessor(0, 0) = right.x;
    cameraMatrix.Accessor(0, 1) = right.y;
    cameraMatrix.Accessor(0, 2) = right.z;
    cameraMatrix.Accessor(1, 0) = up.x;
    cameraMatrix.Accessor(1, 1) = up.y;
    cameraMatrix.Accessor(1, 2) = up.z;
    cameraMatrix.Accessor(2, 0) = -forward.x;
    cameraMatrix.Accessor(2, 1) = -forward.y;
    cameraMatrix.Accessor(2, 2) = -forward.z;

    cameraMatrix.Accessor(0, 3) = -right.Dot(position);
    cameraMatrix.Accessor(1, 3) = -up.Dot(position);
    cameraMatrix.Accessor(2, 3) = forward.Dot(position);

    cameraMatrix.Accessor(3, 3) = 1;

    return cameraMatrix;
}

void Camera::UpdateRotation(const Vec2& newMousePosition)
{
	float dx = mousePosition.x - newMousePosition.x;
	float dy = mousePosition.y - newMousePosition.y;

	yaw += dx / 1000.0f;
	pitch += dy / 1000.0f;

    pitch = pitch > -M_PI_2 ? ( pitch < M_PI_2 ? pitch : M_PI_2 ) : -M_PI_2;

	mousePosition = newMousePosition;
}