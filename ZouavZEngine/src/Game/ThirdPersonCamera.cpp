#include "GameObject.hpp"
#include "Rendering/Camera.hpp"
#include "Game/ThirdPersonCamera.hpp"
#include "System/InputManager.hpp"
#include "System/TimeManager.hpp"

ThirdPersonCamera::ThirdPersonCamera(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void ThirdPersonCamera::Editor()
{

}

void ThirdPersonCamera::OnAddComponent()
{
	GetGameObject().AddComponent<Camera>();
	GetGameObject().GetComponent<Camera>()->SetPosition({ 0, 5, 8 });
	GetGameObject().GetComponent<Camera>()->SetTarget({ 0, 0, 0 });
}

void ThirdPersonCamera::Begin()
{
}

void ThirdPersonCamera::Update()
{
	Vec2 offset = InputManager::GetCursorOffsetFromLastFrame();

	xCameraAngle += offset.y / 10.0f;
	if (xCameraAngle > 89.0f || xCameraAngle < -89.0f)
	{
		xCameraAngle = xCameraAngle > -89.0f ? (xCameraAngle < 89.0f ? xCameraAngle : 89.0f) : -89.0f;
		offset.y = 0;
	}
	GetGameObject().GetParent().Rotate({0.0f, offset.x / 10.0f, 0.0f });
	GetGameObject().Rotate({ offset.y / 10.0f, 0.0f, 0.0f });
}