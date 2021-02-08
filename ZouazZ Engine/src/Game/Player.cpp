#include "Game/Player.hpp"
#include "System/InputManager.hpp"
#include "GameObject.hpp"
#include <iostream>

Player::Player(GameObject* _gameobject)
	: ScriptComponent(_gameobject)
{

}

void Player::Begin()
{
	life = 10;
}

void Player::Update()
{
	if (InputManager::GetKeyState(E_KEYS::SPACEBAR))
		life--;
	if (life <= 0)
		std::cout << "dead" << std::endl;
}
