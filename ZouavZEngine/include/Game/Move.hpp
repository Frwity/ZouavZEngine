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

		const char* GetComponentName() override { return "Move"; }

		template <class Archive>
		void serialize(Archive& ar)
		{
			
		}

		template <class Archive>
		static void load_and_construct(Archive& _ar, cereal::construct<Move>& _construct)
		{
			_construct(GameObject::currentLoadedGameObject);
		}
};


CEREAL_REGISTER_TYPE(Move)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Move)