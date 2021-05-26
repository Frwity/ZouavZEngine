#pragma once

#include "Component/Component.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Bone.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/AnimResource.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include <assimp/scene.h>
#include <memory>

class Animation : public Component
{
private:
    std::shared_ptr<Mesh> mesh;

    float currentTime = 0.0f;

    std::shared_ptr<Shader> animationShader;

public :
    bool play = false;
    AssimpNodeData rootNode;
    std::map<std::string, std::shared_ptr<AnimResource>> animations;
    std::shared_ptr<AnimResource> currentAnimation;

    Component* Clone() const override { return new Animation(*this); };

    Animation() = default;
    Animation(GameObject* _gameObject, std::string _animationPath = std::string(), Mesh* _mesh = nullptr);
    ~Animation() = default;

    void LoadAnimation(std::string _name, std::string _path);

    void Draw(const Camera& _camera);

    void Editor() override;
    const char* GetComponentName() override { return "Animation"; }

    template <class Archive>
    void serialize(Archive& _ar)
    {
        _ar(cereal::base_class<Component>(this));
    }

    template <class Archive>
    static void load_and_construct(Archive& _ar, cereal::construct<Animation>& _construct);
};

CEREAL_REGISTER_TYPE(Animation)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Animation)