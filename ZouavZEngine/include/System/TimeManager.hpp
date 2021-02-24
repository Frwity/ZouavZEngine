#pragma once

#include <memory>
#include <vector>

class Clock
{
	//friend class TimeManager;
private:

	float time = 0.0f;
	float deltaTime = 0.0f;
	float previousTime = 0.0f;
	float scale = 1.0f;

	bool activate = true;

	Clock() = default;

public:

	~Clock() = default;

	float GetTime() { return time; }
	float GetDeltaTime() { return deltaTime * scale; }
	float GetUnscaledDeltaTime() { return deltaTime; }

	float SetScale(float _scale) { scale = _scale > 0 ? _scale : -_scale; }

	void Update(float _deltaTime);

	void Activate() { activate = true; }
	void Dehactivate() { activate = false; }

};

class TimeManager
{
private:

	static std::vector<std::unique_ptr<Clock>> clocks;

	static float time;
	static float deltaTime;
	static float previousTime;
	static float scale;

public:

	static float GetTime() { return time; }
	static float GetDeltaTime() { return deltaTime * scale; }
	static float GetUnscaledDeltaTime() { return deltaTime; }

	static float SetScale(float _scale) { scale = _scale > 0 ? _scale : -_scale; }
	static Clock& CreateClock();
	static void RemoveClock(Clock* _clock);

	static void Update();
};