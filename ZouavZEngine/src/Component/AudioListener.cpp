#include "GameObject.hpp"
#include <AL/al.h>
#include "Component/AudioListener.hpp"

AudioListener::AudioListener(GameObject* _gameObject)
	: Component(_gameObject)
{
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_POSITION, _gameObject->WorldPosition().x, _gameObject->WorldPosition().y, _gameObject->WorldPosition().z);
}

void AudioListener::Update()
{
	alListener3f(AL_POSITION, gameObject->WorldPosition().x, gameObject->WorldPosition().y, gameObject->WorldPosition().z);
}