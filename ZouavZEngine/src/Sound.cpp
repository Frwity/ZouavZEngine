#include <al.h>
#include <alc.h>
#include <sndfile.h>
#include <iostream>
#include <vector>
#include "Sound.hpp"

Sound::Sound(const char* _path)
{
    SF_INFO FileInfos;
    SNDFILE* File = sf_open(_path, SFM_READ, &FileInfos);
    if (!File)
    {
        std::cout << _path << " not loaded";
        return;
    }
    ALsizei NbSamples = static_cast<ALsizei>(FileInfos.channels * FileInfos.frames);
    ALsizei SampleRate = static_cast<ALsizei>(FileInfos.samplerate);

    std::vector<ALshort> Samples(NbSamples);
    if (sf_read_short(File, &Samples[0], NbSamples) < NbSamples)
    {
        std::cout << _path << " not loaded";
        return;
    }

    sf_close(File);

    ALenum Format;
    switch (FileInfos.channels)
    {
        case 1:  Format = AL_FORMAT_MONO16;   break;
        case 2:  Format = AL_FORMAT_STEREO16; break;
        default: std::cout << _path << " not loaded";  return;
    }

    alGenBuffers(1, &buffer);

    // Remplissage avec les échantillons lus
    alBufferData(buffer, Format, &Samples[0], NbSamples * sizeof(ALushort), SampleRate);

    // Vérification des erreurs
    if (alGetError() != AL_NO_ERROR)
    {
        std::cout << _path << " not loaded";
        return;
    }

    alGenSources(1, &source);

    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
    alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcef(source, AL_REFERENCE_DISTANCE, 3.0f);
    alSourcef(source, AL_MAX_DISTANCE, 5.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, buffer);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

Sound::~Sound()
{
    alDeleteBuffers(1, &buffer);

    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
}

void Sound::Play()
{
    alSourcePlay(source);
}

void Sound::Stop()
{
    alSourceStop(source);
}

void Sound::SetPosition(const Vec3& _position)
{
    alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
}

void Sound::SetLooping(bool _loop)
{
    alSourcei(source, AL_LOOPING, _loop);
}