#pragma once

#include "Resource.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Bone.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include <assimp/scene.h>
#include "Rendering/Shader.hpp"
#include "Rendering/Camera.hpp"
#include <memory>

struct AssimpNodeData
{
    Mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class AnimResource: public Resource
{
private:
    std::shared_ptr<Shader> animationShader;
    std::vector<Bone> bones;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;
    float currentTime = 0.0f;

    void ReadMissingBones(const aiAnimation* animation);
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
    Bone* FindBone(const std::string& _boneName);
    void CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform);

public:
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;
    std::vector<Mat4> finalBonesMatrices;
    float duration;
    int tickPerSecond;
    bool loop = true;

    AnimResource(const std::string& _name, std::string& _path, std::shared_ptr<Mesh> _mesh = nullptr);
    ~AnimResource() = default;

    void RemoveFromResourcesManager() override;

    void UpdateAnimation(float _deltaTime);
};