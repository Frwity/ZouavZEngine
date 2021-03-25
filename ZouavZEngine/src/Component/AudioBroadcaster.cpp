#include "GameObject.hpp"
#include "Sound.hpp"
#include "Component/AudioBroadcaster.hpp"
#include "imgui.h"
#include "System/ResourcesManager.hpp"

AudioBroadcaster::AudioBroadcaster(GameObject* _gameObject)
	: Component(_gameObject)
{}

AudioBroadcaster::AudioBroadcaster(GameObject* _gameObject, Sound* _sound)
	: Component(_gameObject), sound{_sound}
{}

void AudioBroadcaster::Update()
{
	if (!ambient && sound)
		sound->SetPosition(gameObject->WorldPosition());
}

void AudioBroadcaster::Play()
{
	if (sound)
		sound->Play();
}

void AudioBroadcaster::Stop()
{
	if (sound)
		sound->Stop();
}

void AudioBroadcaster::SetLooping(bool _loop)
{
	if (sound)
	{
		sound->SetLooping(_loop);
		loop = _loop;
	}
}

void AudioBroadcaster::SetAmbient(bool _ambient)
{
	if (sound)
	{
		sound->SetAmbient(_ambient);
		sound->SetPosition(Vec3::zero);
		ambient = _ambient;
	}
}

void AudioBroadcaster::Editor()
{
	ImGui::Text("AudioBroadcaster");
	ResourcesManager::ResourceChanger<Sound>("Sound", sound);

	ImGui::Text("Ambient : "); 
	ImGui::SameLine();
	if (ImGui::Checkbox("##ambient", &ambient))
		SetAmbient(ambient);

	ImGui::Text("Loop    : "); 
	ImGui::SameLine();
	if (ImGui::Checkbox("##loop", &loop))
		SetAmbient(loop);
}