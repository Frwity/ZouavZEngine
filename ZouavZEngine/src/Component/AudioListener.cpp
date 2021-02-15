#include "GameObject.hpp"
#include <al.h>
#include "Component/AudioListener.hpp"

AudioListener::AudioListener(GameObject* _gameObject)
	: Component(_gameObject)
{
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_POSITION, _gameObject->position.x, _gameObject->position.y, _gameObject->position.z);
}

void AudioListener::Update()
{
	alListener3f(AL_POSITION, gameObject->position.x, gameObject->position.y, gameObject->position.z);
}