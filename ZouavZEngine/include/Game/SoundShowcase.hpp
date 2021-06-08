#pragma once
#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class SoundShowcase : public ScriptComponent
{
   friend class cereal::access;
private:
public:
    std::string name;
    class AudioBroadcaster* audioBroadcaster;
    SoundShowcase() = delete;
    SoundShowcase(class GameObject* _gameobject, std::string _name = "SoundShowcase");
    void Begin() override;
    void Update() override;
    void Editor() override;

    template <class Archive>
    void serialize(Archive & _ar)
    {
        _ar(cereal::base_class<ScriptComponent>(this));
    }
    template <class Archive>
    static void load_and_construct(Archive & _ar, cereal::construct<SoundShowcase>&_construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<ScriptComponent>(_construct.ptr()));
    }
};
CEREAL_REGISTER_TYPE(SoundShowcase)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, SoundShowcase)