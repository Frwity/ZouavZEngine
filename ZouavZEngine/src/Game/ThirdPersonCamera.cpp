#include "GameObject.hpp"
#include "Component/Transform.hpp"

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
	camera = GetGameObject().AddComponent<Camera>();
	camera->SetPosition({ 0, 5, -8 });
	camera->SetTarget({ 0, 0, 0 });
}

void ThirdPersonCamera::Begin()
{
	camera = GetGameObject().GetComponent<Camera>();
	oldMousePos = InputManager::GetCursorPos();
}

void ThirdPersonCamera::Update()
{
	//sceneCamera.pitch = sceneCamera.pitch > -(float)M_PI_2 ? (sceneCamera.pitch < (float)M_PI_2 ? sceneCamera.pitch : (float)M_PI_2) : -(float)M_PI_2;

	Vec2 newMousePos = InputManager::GetCursorPos();
	Vec2 offset = newMousePos - oldMousePos;
	static Vec2 movement;
	static Vec3 basePos = camera->GetPosition();
	Transform t(basePos, {0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f });
	movement += offset;

	//Mat4::CreateTRSMatrix({ 1, 1, 1 }, { movement.y, movement.x, 0 }, { 1, 1, 1 });

	oldMousePos = newMousePos;
}