#pragma once
#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class ThirdPersonCamera : public ScriptComponent
{
   friend class cereal::access;
private:
    float xCameraAngle = 0.0f;

public:
    ThirdPersonCamera() = delete;
    ThirdPersonCamera(class GameObject* _gameobject, std::string _name = "ThirdPersonCamera");
    void Begin() override;
    void OnAddComponent() override;
    void Update() override;
    void Editor() override;

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(cereal::base_class<ScriptComponent>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<ThirdPersonCamera>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<ScriptComponent>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(ThirdPersonCamera)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, ThirdPersonCamera)