#pragma once
#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

class Move : public ScriptComponent
{
   public:
        Move() = delete;
        Move(class GameObject* _gameobject);
        void Begin() final;
        void Update() final;
};

//
//CEREAL_REGISTER_TYPE(Move)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Move)