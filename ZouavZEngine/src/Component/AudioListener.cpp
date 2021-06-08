#include "GameObject.hpp"
#include <AL/al.h>
#include "System/SoundManager.hpp"
#include "Component/AudioListener.hpp"
#include "imgui.h"

AudioListener::AudioListener(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name)
{
	SoundManager::SetListenerVelocity({ 0.0f, 0.0f, 0.0f });
	if (_gameObject)
		SoundManager::SetListenerPosition(_gameObject->WorldPosition());
	SoundManager::SetListener(this);
}

AudioListener::~AudioListener()
{
	SoundManager::RemoveListener(this);
}

void AudioListener::Update()
{
	SoundManager::SetListenerPosition(GetGameObject().WorldPosition());
	float values[6] = {GetGameObject().Forward().x, GetGameObject().Forward().y, GetGameObject().Forward().z, GetGameObject().Up().x, GetGameObject().Up().y, GetGameObject().Up().z };
	alListenerfv(AL_ORIENTATION, values);
}

void AudioListener::Editor()
{
}

template <class Archive>
static void AudioListener::load_and_construct(Archive& _ar, cereal::construct<AudioListener>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(cereal::base_class<Component>(_construct.ptr()));
}