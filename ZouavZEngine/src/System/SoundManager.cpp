#include <al.h>
#include <alc.h>
#include <iostream>
#include "System/SoundManager.hpp"

SoundManager::SoundManager()
{
    ALCdevice* Device = alcOpenDevice(nullptr);
    if (!Device)
    {
        std::cout << "Failed load OpenAL" << std::endl;
        return;
    }

    ALCcontext* Context = alcCreateContext(Device, nullptr);
    if (!Context)
    {
        std::cout << "Failed load OpenAL" << std::endl;
        return;
    }

    if (!alcMakeContextCurrent(Context))
    {
        std::cout << "Failed load OpenAL" << std::endl;
        return;
    }

    float values[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, values);
}

SoundManager::~SoundManager()
{
    ALCcontext* Context = alcGetCurrentContext();
    ALCdevice* Device = alcGetContextsDevice(Context);

    alcMakeContextCurrent(nullptr);

    alcDestroyContext(Context);

    alcCloseDevice(Device);
}