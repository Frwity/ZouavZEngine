#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Mat4.hpp"
#include "Component/Component.hpp"

class Camera : public Component
{
private:
	static Camera* mainCamera;

protected:
	Mat4 projection;
	Vec3 position;
	Vec3 target;

public:
	Camera(class GameObject* _gameObject, int _width, int _height);
	Camera() = delete;
	~Camera() = default;

	static const Camera* GetMainCamera() { return mainCamera; }

	void SetMainCamera() { mainCamera = this; }
	
	void SetPosition(const Vec3& _pos) { position = _pos; }
	void SetTarget(const Vec3& _target) { target = _target; }

	virtual Mat4 GetMatrix() const;

	Mat4 GetProjetionMatrix() const { return projection; }
};

class SceneCamera : public Camera
{
private:
	static SceneCamera* sceneCamera;

	Vec2 mousePosition{ 0.0f, 0.0f };
	float pitch{ 0.0f };
	float yaw{ 0.0f };
	float speed{ 0.0f };

public:
	SceneCamera(int _width, int _height);
	SceneCamera() = default;
	~SceneCamera() = default;

	static const Camera* GetSceneCamera() { return sceneCamera; }

	void SetSceneCamera() { sceneCamera = this; }

	float Speed() const;

	void MoveTo(const Vec3& _direction);
	Mat4 GetMatrix() const final;
	Mat4 GetLookAtMatrix(const Vec3& _target) const;
	void UpdateRotation(const Vec2& _newMousePosition);

	void Update(bool _isKeyboardEnable);
};