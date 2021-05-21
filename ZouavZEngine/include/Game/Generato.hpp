#pragma once
#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"
#include "Prefab.hpp"

class Generato : public ScriptComponent
{
public:
    Prefab prefab;
    Generato() = delete;
    Generato(class GameObject* _gameobject, std::string _name = "Generato");
    void Editor() override;
    void Begin() final;
    void Update() final;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Generato>& _construct);
};

CEREAL_REGISTER_TYPE(Generato)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ScriptComponent, Generato)