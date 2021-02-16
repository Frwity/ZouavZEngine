#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include "Sound.hpp"
#include "System/SoundManager.hpp"

float SoundManager::mainVolume = 1.0f;
std::vector<Sound*> SoundManager::sounds;

void SoundManager::Init()
{
    ALCdevice* Device = alcOpenDevice(nullptr);
    if (!Device)
    {
        std::cout << "OpenAL Error : Open Device failed" << std::endl;
        return;
    }

    ALCcontext* Context = alcCreateContext(Device, nullptr);
    if (!Context)
    {
        std::cout << "OpenAL Error : Create Context failed" << std::endl;
        return;
    }

    if (!alcMakeContextCurrent(Context))
    {
        std::cout << "OpenAL Error : Make Context failed" << std::endl;
        return;
    }

    float values[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, values);

}

void SoundManager::Update()
{
    for (Sound* sound : sounds)
        sound->SetVolume(mainVolume);
}

void SoundManager::Destroy()
{
    ALCcontext* Context = alcGetCurrentContext();
    ALCdevice* Device = alcGetContextsDevice(Context);

    alcMakeContextCurrent(nullptr);

    alcDestroyContext(Context);

    alcCloseDevice(Device);
}

void SoundManager::AddSound(Sound* _newSound)
{
    sounds.push_back(_newSound);
}

void SoundManager::RemoveSound(Sound* _newSound)
{
    for (auto it = sounds.begin(); it != sounds.end(); )
    {
        if (*it == _newSound)
        {
            sounds.erase(it);
            return;
        }
        else
            ++it;
    }
}