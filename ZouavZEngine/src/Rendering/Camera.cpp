#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Camera.hpp"
#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

// TODO : split in different file

Camera* Camera::mainCamera = nullptr;
SceneCamera* SceneCamera::sceneCamera = nullptr;

Camera::Camera(class GameObject* _gameObject, int _width, int _height)
    : Component(_gameObject)
{
    if (!mainCamera)
        mainCamera = this;

    target = { 0.0f, 0.0f, 0.0f };
    position = { 0.0f, 0.0f, 0.0f };
    projection = Mat4::CreatePerspectiveProjectionMatrix((float)_width, (float)_height, CAMERA_NEAR, CAMERA_FAR, CAMERA_FOV);
}

Camera::~Camera()
{
    if (mainCamera == this)
        mainCamera = nullptr;
}

Mat4 Camera::GetMatrix() const
{
    const Vec3 pos = gameObject->WorldPosition() + position;
    const Vec3 targetPos = gameObject->WorldPosition() + target;

    const Vec3 forward = (targetPos - pos).Normalized();
    const Vec3 right = (forward.Cross(Vec3::up)).Normalized();
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

    cameraMatrix.Accessor(0, 3) = -right.Dot(pos);
    cameraMatrix.Accessor(1, 3) = -up.Dot(pos);
    cameraMatrix.Accessor(2, 3) = forward.Dot(pos);

    cameraMatrix.Accessor(3, 3) = 1;

    return followGameObjectRotation ? gameObject->WorldRotation().GetRotationMatrix() * cameraMatrix.Reversed() : cameraMatrix.Reversed();
}

void Camera::Resize(int _width, int _height)
{
    projection = Mat4::CreatePerspectiveProjectionMatrix((float)_width, (float)_height, CAMERA_NEAR, CAMERA_FAR, CAMERA_FOV);
}

SceneCamera::SceneCamera(int _width, int _height)
    : Camera(nullptr, _width, _height), mousePosition(0.0f, 0.0f), pitch(0.0f), yaw((float)M_PI), speed{ 30.0f }
{
    if (!sceneCamera)
        sceneCamera = this;
}

float SceneCamera::Speed() const
{
    return speed;
}

void SceneCamera::MoveTo(const Vec3& _direction)
{
    position += Vec3((sin(yaw) / 10) * _direction.z + (cos(yaw) / 10) * _direction.x, _direction.y / 10 - (sin(pitch) / 10) * _direction.z, (cos(yaw) / 10) * _direction.z - (sin(yaw) / 10) * _direction.x);
}

Mat4 SceneCamera::GetMatrix() const
{
    return Mat4::CreateTRSMatrix(position, { pitch, yaw, 0 }, { 1, 1, 1 });
}

Mat4 SceneCamera::GetLookAtMatrix(const Vec3& _target) const
{
    const Vec3 forward = (_target - position).Normalized();
    const Vec3 right = (forward.Cross(Vec3::up)).Normalized();
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

void SceneCamera::UpdateRotation(const Vec2& _newMousePosition)
{
	float dx = mousePosition.x - _newMousePosition.x;
    float dy = mousePosition.y - _newMousePosition.y;
    yaw += dx / 1000.0f;
    pitch += dy / 1000.0f;

    pitch = pitch > -(float)M_PI_2 ? (pitch < (float)M_PI_2 ? pitch : (float)M_PI_2) : -(float)M_PI_2;

    mousePosition = _newMousePosition;
} 

void SceneCamera::Update(bool _isKeyboardEnable, float _deltaTime)
{
    bool sprint = InputManager::GetKeyPressed(E_KEYS::LCTRL);
    float cameraSpeed = _deltaTime * Speed() + Speed() * sprint * 1.2f;

    if (InputManager::GetKeyPressed(E_KEYS::W))
        MoveTo({ 0.0f, 0.0f, -cameraSpeed });

    if (InputManager::GetKeyPressed(E_KEYS::S))
        MoveTo({ 0.0f, 0.0f, cameraSpeed });

    if (InputManager::GetKeyPressed(E_KEYS::D))
        MoveTo({ cameraSpeed, 0.0f, 0.0f });

    if (InputManager::GetKeyPressed(E_KEYS::A))
        MoveTo({ -cameraSpeed, 0.0f, 0.0f });

    if (InputManager::GetKeyPressed(E_KEYS::SPACEBAR))
        MoveTo({ 0.0f, cameraSpeed, 0.0f });

    if (InputManager::GetKeyPressed(E_KEYS::LSHIFT))
        MoveTo({ 0.0f, -cameraSpeed, 0.0f });

    if (!_isKeyboardEnable)
        return;

    UpdateRotation(InputManager::GetCursorPos());
}