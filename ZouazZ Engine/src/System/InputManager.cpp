#include "System/InputManager.hpp"
#include <GLFW/glfw3.h>

GLFWwindow* InputManager::window = nullptr;

std::unordered_map<E_MOUSE_BUTTON, std::pair<bool, bool>> InputManager::mouseButtonsState;

void InputManager::InitMouseButtons()
{
	for (int i = 0; i < static_cast<int>(E_MOUSE_BUTTON::NUMBER_OF_BUTTONS); i++)
		mouseButtonsState.emplace(static_cast<E_MOUSE_BUTTON>(i), std::make_pair(false, false));
}

void InputManager::UpdateMouseButtons()
{
	for (auto& a : mouseButtonsState)
	{
		a.second.second = a.second.first;
		a.second.first = glfwGetMouseButton(window, static_cast<int>(a.first)) == GLFW_PRESS;
	}
}

bool InputManager::GetKeyState(E_KEYS _key)
{
	return glfwGetKey(window, static_cast<int>(_key)) == GLFW_PRESS;
}

bool InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON _button)
{
	return mouseButtonsState[_button].first;
}

bool InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON _button)
{
	auto& mouseButton = mouseButtonsState[_button];
	return mouseButton.first && !mouseButton.second;
}

bool InputManager::GetMouseButtonReleased(E_MOUSE_BUTTON _button)
{
	return !mouseButtonsState[_button].first;
}

bool InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON _button)
{
	auto& mouseButton = mouseButtonsState[_button];
	return !mouseButton.first && mouseButton.second;
}

void InputManager::SetWindow(GLFWwindow* _window)
{
	window = _window;
}