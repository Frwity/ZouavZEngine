#include "Game/Player.hpp"
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
	if (life <= 0)
		std::cout << "dead" << std::endl;
}
