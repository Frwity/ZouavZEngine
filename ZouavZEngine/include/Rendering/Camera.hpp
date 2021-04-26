#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"
#include "Maths/Mat4.hpp"
#include "Component/Component.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"
#include <iostream>

class Camera : public Component
{
private:
	friend cereal::access;
	static Camera* mainCamera;
	bool followGameObjectRotation = false;

protected:

	Mat4 projection;
	Vec3 position;
	Vec3 target;
	int width = 0;
	int height = 0;
	float near = 0.01f;
	float far = 5000.0f;
	float fov = 45.0f;

public:
	Camera(class GameObject* _gameObject, int _width, int _height);
	Camera() = delete;
	Camera(const Camera&) = delete;
	~Camera();

	void Editor() override;

	const char* GetComponentName() override { return "Camera"; }

	static /*const */Camera* GetMainCamera() { return mainCamera; }

	void SetMainCamera() { mainCamera = this; }
	
	void SetPosition(const Vec3& _pos) { position = _pos; }
	void SetTarget(const Vec3& _target) { target = _target; }

	virtual Mat4 GetMatrix() const;

	void Resize(int _width, int _height);

	Mat4 GetProjetionMatrix() const { return projection; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(projection.matrix[0], projection.matrix[1], projection.matrix[2], projection.matrix[3],
			projection.matrix[4], projection.matrix[5], projection.matrix[6], projection.matrix[7],
			projection.matrix[8], projection.matrix[9], projection.matrix[10], projection.matrix[11],
			projection.matrix[12], projection.matrix[13], projection.matrix[14], projection.matrix[15],
			position.x, position.y, position.z,
			target.x, target.y, target.z, mainCamera == this ? true : false);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Camera>& _construct);
};

class SceneCamera : public Camera
{
private:
	friend cereal::access;

	static SceneCamera* sceneCamera;

	Vec2 mousePosition{ 0.0f, 0.0f };
	float pitch{ 0.0f };
	float yaw{ 0.0f };
	float speed{ 0.0f };

public:
	SceneCamera(int _width, int _height);
	SceneCamera() = default;
	~SceneCamera() = default;

	static /*const */Camera* GetSceneCamera() { return sceneCamera; }

	void SetSceneCamera() { sceneCamera = this; }

	float Speed() const;

	void MoveTo(const Vec3& _direction);
	Mat4 GetMatrix() const final;
	Mat4 GetLookAtMatrix(const Vec3& _target) const;
	void UpdateRotation(const Vec2& _newMousePosition);

	void Update(bool _isKeyboardEnable, float _deltaTime);

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(projection.matrix[0], projection.matrix[1], projection.matrix[2], projection.matrix[3],
			projection.matrix[4], projection.matrix[5], projection.matrix[6], projection.matrix[7],
			projection.matrix[8], projection.matrix[9], projection.matrix[10], projection.matrix[11],
			projection.matrix[12], projection.matrix[13], projection.matrix[14], projection.matrix[15],
			position.x, position.y, position.z,
			target.x, target.y, target.z,
			mousePosition.x, mousePosition.y,
			pitch, yaw, speed);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<SceneCamera>& _construct)
	{
		Mat4 _projection;
		Vec3 _position;
		Vec3 _target;
		Vec2 _mousePosition;
		float _pitch, _yaw, _speed;
		_ar(_projection.matrix[0], _projection.matrix[1], _projection.matrix[2], _projection.matrix[3],
			_projection.matrix[4], _projection.matrix[5], _projection.matrix[6], _projection.matrix[7],
			_projection.matrix[8], _projection.matrix[9], _projection.matrix[10], _projection.matrix[11],
			_projection.matrix[12], _projection.matrix[13], _projection.matrix[14], _projection.matrix[15],
			_position.x, _position.y, _position.z,
			_target.x, _target.y, _target.z,
			_mousePosition.x, _mousePosition.y,
			_pitch, _yaw, _speed);
		_construct(10, 10);
		_construct->projection = _projection;
		_construct->position = _position;
		_construct->target = _target;
		_construct->mousePosition = _mousePosition;
		_construct->pitch = _pitch;
		_construct->yaw = _yaw;
		_construct->speed = _speed;
	}
};

CEREAL_REGISTER_TYPE(Camera)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Camera)
CEREAL_REGISTER_TYPE(SceneCamera)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, SceneCamera)