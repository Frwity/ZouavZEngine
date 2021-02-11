#pragma once

class TimeManager
{
private:
	static float time;
	static float deltaTime;
	static float previousTime;
	static float scale;

	TimeManager() = delete;
	~TimeManager() = delete;

public:
	static float GetTime() { return time; }
	static float GetDeltaTime() { return deltaTime * scale; }
	static float GetUnscaledDeltaTime() { return deltaTime; }

	static float SetScale(float _scale) { scale = _scale > 0 ? _scale : 0.0f - _scale; }

	static void Update();
};