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
    std::vector<Bone> bones;
    Mesh* mesh;

    void ReadMissingBones(const aiAnimation* animation);
    void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
    Bone* FindBone(const std::string& _boneName);
    void CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform, float _currentTime);

public:
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;
    std::vector<Mat4> finalBonesMatrices;
    float duration;
    int tickPerSecond;
    float animationSpeed = 0.2f;
    bool loop = false;
    bool goToIdle = true;

    AnimResource(const std::string& _name, std::string& _path, Mesh* _mesh = nullptr);
    ~AnimResource() = default;

    void UpdateAnimationResources(Mesh* _mesh = nullptr);

    void RemoveFromResourcesManager() override;

    void UpdateAnimation(float _deltaTime, bool _loop, float& _currentTime, bool& _animationFinish);
};