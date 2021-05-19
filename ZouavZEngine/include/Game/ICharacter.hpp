#pragma once

#include "Component/ScriptComponent.hpp"
#include "Maths/Vec4.hpp"

#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class ICharacter : public ScriptComponent
{
protected:
    friend class cereal::access;

    class Material* material = nullptr;
    class FontComponent* lifeFont = nullptr;
	Vec4 baseColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    Vec4 damageColor{ 1.0f, 0.0f, 0.0f, 1.0f };
    float invulnerabilityFrame = 1.0f;
    float invulnerabilityTimer = 0.0f;
    int life = 5;
    bool asTakenDamage = false;

public:
    ICharacter() = delete;
    ICharacter(class GameObject* _gameobject);
    void Begin() override;
    void Update() override;
    const char* GetComponentName() override { return "Character"; }
    void Editor() override;


    int GetLife() const { return life; }
    bool IsAlive() const { return life > 0; }
    bool Damage(int _damage);

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(damageColor, invulnerabilityFrame, life);
        _ar(cereal::base_class<Component>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<ICharacter>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(_construct->invulnerabilityFrame, _construct->life);
        _ar(cereal::base_class<Component>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(ICharacter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, ICharacter)