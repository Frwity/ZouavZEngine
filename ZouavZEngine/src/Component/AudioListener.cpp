#include "GameObject.hpp"
#include <AL/al.h>
#include "System/SoundManager.hpp"
#include "Component/AudioListener.hpp"

AudioListener::AudioListener(GameObject* _gameObject)
	: Component(_gameObject)
{
	SoundManager::SetListenerVelocity({ 0.0f, 0.0f, 0.0f });
	if (_gameObject)
		SoundManager::SetListenerPosition(_gameObject->WorldPosition());
}

void AudioListener::Update()
{
	SoundManager::SetListenerPosition(gameObject->WorldPosition());
}