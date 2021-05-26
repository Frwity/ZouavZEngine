#pragma once
#include "Game/ICharacter.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Enemy : public ICharacter
{
   friend class cereal::access;
private:
    GameObject* player = nullptr;
    float detectionDistance = 25.0f;
    float distanceToAttack = 2.0f;
    float distanceToStop = 1.5f;
    float speed = 2.0f;

public:
    Enemy() = delete;
    Enemy(class GameObject* _gameobject, std::string _name = "Enemy");
    void Begin() override;
    void Update() override;
    void Editor() override;

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(cereal::base_class<ICharacter>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<Enemy>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<ICharacter>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(Enemy)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ICharacter, Enemy)