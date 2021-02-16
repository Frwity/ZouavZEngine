#pragma once
#include "Component/ScriptComponent.hpp"

class Move : public ScriptComponent
{
   public:
        Move() = delete;
        Move(class GameObject* _gameobject);
        void Begin() final;
        void Update() final;
};