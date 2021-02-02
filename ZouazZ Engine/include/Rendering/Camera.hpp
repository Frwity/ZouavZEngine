#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Mat4.hpp"

class Camera
{
private:
	static Camera* mainCamera;
	Mat4 projection;
	Vec2 mousePosition;
	Vec3 position;
	float pitch, yaw;
	float speed;

public:
	Camera(const Vec2& mouseStartPosition, int width, int height);
	Camera() = default;
	~Camera() = default;

	static const Camera& GetMainCamera() { return *mainCamera; }

	void SetMainCamera() { mainCamera = this; }

	float Speed() const;

	void MoveTo(const Vec3& direction);
	Mat4 GetMatrix() const;
	Mat4 GetLookAtMatrix(const Vec3& target) const;
	void UpdateRotation(const Vec2& newMousePosition);

	Mat4 GetProjetionMatrix() const { return projection; }

};