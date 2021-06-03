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
    std::shared_ptr<Shader> animationShader;
public :
    bool play = false;
    std::unordered_map<std::string, std::shared_ptr<AnimResource>> animationsAttached;
    std::shared_ptr<AnimResource> currentAnimation;
    Texture* text;
    Mesh* mesh;
    float animationSpeed = 0.2f;
    bool loop = true;

    Animation() = default;
    Animation(GameObject* _gameObject, std::string _animationPath = std::string(), Mesh* _mesh = nullptr, std::string _name = "Animation");
    ~Animation() = default;

    void Draw(const Camera& _camera);

    void Editor() override;
    void Play() { play = true; };
    void Stop() { play = false; }

    template <class Archive>
    void serialize(Archive& _ar)
    {
        //if (currentAnimation)
        //    _ar(currentAnimation->GetName());
        _ar(animationsAttached.size());
        for (std::pair<std::string, std::shared_ptr<AnimResource>> element : animationsAttached)
            _ar(element.second->GetName());

        _ar(cereal::base_class<Component>(this));
    }

    template <class Archive>
    static void load_and_construct(Archive& _ar, cereal::construct<Animation>& _construct);
};

CEREAL_REGISTER_TYPE(Animation)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Animation)