#include "GameObject.hpp"
#include "Game/EnemyManager.hpp"
#include "Game/Player.hpp"
#include "Game/Enemy.hpp"
#include "Component/MeshRenderer.hpp"
#include "System/Terrain.hpp"
#include "System/Debug.hpp"
#include <imgui.h>

std::vector<class Enemy*> EnemyManager::enemies;


EnemyManager::EnemyManager(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void EnemyManager::AddEnemy(Enemy* _enemy)
{
    enemies.push_back(_enemy);
}

void EnemyManager::RemoveEnemy(Enemy* _enemy)
{
    for (auto it = enemies.begin(); it != enemies.end(); )
    {
        if (*it == _enemy)
        {
            enemies.erase(it);
            return;
        }
        else
            ++it;
    }
}

void EnemyManager::Editor()
{
    if (ImGui::Button("Add Enemy Type"))
    {
        ++nbEnemiesPrefab;
        enemiesPrefab.emplace_back(Prefab{});
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove Enemy Type"))
    {
        --nbEnemiesPrefab;
        enemiesPrefab.pop_back();
    }
    for (int i = 0; i < nbEnemiesPrefab; ++i)
    {
        ImGui::PushID(0);
        enemiesPrefab[i].Editor("Enemy Prefab :");
        ImGui::PopID();
    }
    std::vector<Prefab> enemiesPrefab;
    spawner.Editor("Spawner Prefab :");
    cornerPillar.Editor("Pillar Prefab");
}

void EnemyManager::Begin()
{
    player = GameObject::GetGameObjectByTag("Player");
    playerComp = player->GetComponent<Player>();
    playerChunkPos = Terrain::GetChunkPosFromWorldPos(player->WorldPosition());
    chunkSize = Terrain::GetChunkSize();

    playerChunkPos = Terrain::GetChunkPosFromWorldPos(player->WorldPosition());

    // create first arena
    for (GameObject* go : arenaObject)
        go->Destroy();
    arenaObject.clear();
    currentChunkWorldPos = playerChunkPos * chunkSize;

    arenaObject.emplace_back(GameObject::Instanciate(*spawner, { currentChunkWorldPos.x + chunkSize / 2.f, 40.f, currentChunkWorldPos.y + chunkSize / 2.f }))->SetNotToSave(true);
    arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x, 0.f, currentChunkWorldPos.y }))->SetNotToSave(true);
    arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x + chunkSize, 0.f, currentChunkWorldPos.y }))->SetNotToSave(true);
    arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x + chunkSize, 0.f, currentChunkWorldPos.y + chunkSize }))->SetNotToSave(true);
    arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x, 0.f, currentChunkWorldPos.y + chunkSize }))->SetNotToSave(true);
}

void EnemyManager::Update()
{
    if (!isSpawningDone)
    {   // Spawn enemies
        isSpawningDone = true;
        if (nbEnemiesPrefab > 0)
            enemies.emplace_back(GameObject::Instanciate(*enemiesPrefab[0], { currentChunkWorldPos.x + chunkSize / 2.f, 40.f, currentChunkWorldPos.y + chunkSize / 2.f })->GetComponent<Enemy>());
    }
    else if (asDoneChunk)
    {
        if (Terrain::GetChunkPosFromWorldPos(player->WorldPosition()) != playerChunkPos)
        {
            playerChunkPos = Terrain::GetChunkPosFromWorldPos(player->WorldPosition());
            asDoneChunk = false;
            isSpawningDone = false;

            // reset arena
            for (GameObject* go : arenaObject)
                go->Destroy();
            arenaObject.clear();

            currentChunkWorldPos = playerChunkPos * chunkSize;

            arenaObject.emplace_back(GameObject::Instanciate(*spawner, { currentChunkWorldPos.x + chunkSize / 2.f, 40.f, currentChunkWorldPos.y + chunkSize / 2.f }))->SetNotToSave(true);
            arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x, 0.f, currentChunkWorldPos.y }))->SetNotToSave(true);
            arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x + chunkSize, 0.f, currentChunkWorldPos.y }))->SetNotToSave(true);
            arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x + chunkSize, 0.f, currentChunkWorldPos.y + chunkSize }))->SetNotToSave(true);
            arenaObject.emplace_back(GameObject::Instanciate(*cornerPillar, { currentChunkWorldPos.x, 0.f, currentChunkWorldPos.y + chunkSize }))->SetNotToSave(true);
        }
    }
    else
    {   // remove dead enemy
        for (auto iter = enemies.begin(); iter != enemies.end();) 
            if (!(*iter)->IsAlive())
                iter = enemies.erase(iter);
            else
                iter++;
        if (isSpawningDone && enemies.size() == 0)
        {
            asDoneChunk = true;
            for (auto obj : arenaObject)
                obj->GetComponent<MeshRenderer>()->material.color = { 0.0f, 1.0f, 0.0f, 1.0f };
        }
    }
}

