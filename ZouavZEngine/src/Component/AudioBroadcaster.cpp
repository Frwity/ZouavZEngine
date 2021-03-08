#include "GameObject.hpp"
#include "Sound.hpp"
#include "Component/AudioBroadcaster.hpp"

AudioBroadcaster::AudioBroadcaster(GameObject* _gameObject, Sound* _sound)
	: Component(_gameObject), sound{_sound}
{}

void AudioBroadcaster::Update()
{
	if (updatePosition && sound)
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
		sound->SetLooping(_loop);
}

void AudioBroadcaster::SetAmbient(bool _ambient)
{
	if (sound)
	{
		sound->SetAmbient(_ambient);
		sound->SetPosition(Vec3::zero);
		updatePosition = !_ambient;
	}
}