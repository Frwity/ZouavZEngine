#include "GameObject.hpp"
#include "Game/EnemyManager.hpp"
#include "Game/Player.hpp"


EnemyManager::EnemyManager(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void EnemyManager::Editor()
{}

void EnemyManager::Begin()
{
    player = GameObject::GetGameObjectByTag("Player");
    playerComp = player->GetComponent<Player>();
}

void EnemyManager::Update()
{}