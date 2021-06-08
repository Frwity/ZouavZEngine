#include "GameObject.hpp"
#include "Component/AudioBroadcaster.hpp"
#include <imgui.h>
#include <imgui_stdlib.h>
#include "Game/SoundShowcase.hpp"

SoundShowcase::SoundShowcase(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void SoundShowcase::Editor()
{
   ScriptComponent::Editor();
   ImGui::InputText("Sound Name : ", &name);
}

void SoundShowcase::Begin()
{
   ScriptComponent::Begin();
   GetGameObject().GetComponent<AudioBroadcaster>()->Play(name);
}

void SoundShowcase::Update()
{
   ScriptComponent::Update();
}