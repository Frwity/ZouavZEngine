#pragma once

/*#include "Resource.hpp"
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
    std::shared_ptr<Shader> animationShader;
};

class AnimResource: public Resource
{
private:
    std::shared_ptr<Shader> animationShader;
    std::vector<Bone> bones;
    void CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform);

public:
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;
    std::vector<Mat4> finalBonesMatrices;
    float duration;
    int tickPerSecond;

    AnimResource(const std::string& _name, std::string& _path);
    ~AnimResource();

    void RemoveFromResourcesManager() override;

    void Draw(const Camera& _camera);
};*/