#pragma once

#include "Component/Component.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Bone.hpp"
#include "Rendering/Mesh.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Shader.hpp"
#include <assimp/scene.h>
#include "Rendering/Camera.hpp"
#include <memory>

struct AssimpNodeData
{
    Mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation : public Component
{
private:
    std::vector<Bone> bones;

    std::shared_ptr<Mesh> mesh;
    
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

    float currentTime;

    void CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform);

    std::shared_ptr<Shader> animationShader;

public :
    bool play = false;
    int tickPerSecond;
    float duration;
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;
    std::vector<Mat4> finalBonesMatrices;

    Component* Clone() const override { return new Animation(*this); };

    Animation() = default;
    Animation(GameObject* _gameObject, std::string _animationPath = std::string(), Mesh* _mesh = nullptr);
    ~Animation() = default;

    void UpdateAnimation();

    void ReadMissingBones(const aiAnimation* animation);

    void Draw(const Camera& _camera);

    void Editor() override;
    const char* GetComponentName() override { return "Animation"; }

    Bone* FindBone(const std::string& _boneName);

    template <class Archive>
    void serialize(Archive& _ar)
    {
        _ar(cereal::base_class<Component>(this));
    }

    template <class Archive>
    static void load_and_construct(Archive& _ar, cereal::construct<Animation>& _construct)
    {
        _construct(GameObject::currentLoadedGameObject);
        _ar(cereal::base_class<Component>(_construct.ptr()));
    }
};

CEREAL_REGISTER_TYPE(Animation)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Animation)