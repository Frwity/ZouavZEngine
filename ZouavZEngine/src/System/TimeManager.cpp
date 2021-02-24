#include "System/TimeManager.hpp"
#include <GLFW/glfw3.h>

std::vector<std::unique_ptr<Clock>> TimeManager::clocks;
float TimeManager::time = (float)(glfwGetTime());
float TimeManager::deltaTime = (float)(glfwGetTime());
float TimeManager::previousTime = 0.0f;
float TimeManager::scale = 1.0f;


Clock& TimeManager::CreateClock()
{
	clocks.emplace_back();

	return *clocks.back();
}

void TimeManager::RemoveClock(Clock* _clock)
{
	for (auto it = clocks.begin(); it != clocks.end(); )
	{
		if (it->get() == _clock)
		{
			clocks.erase(it);
			return;
		}
		else
			++it;
	}
}

void TimeManager::Update()
{
	time = (float)(glfwGetTime());
	deltaTime = time - previousTime;
	previousTime = time;

	for (const std::unique_ptr<Clock>& clock : clocks)
		clock->Update(deltaTime);
}
 

void Clock::Update(float _deltaTime)
{
	if (activate)
	{
		time += _deltaTime;
		deltaTime = time - previousTime;
		previousTime = time;
	}
}
