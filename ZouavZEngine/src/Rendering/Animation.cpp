#pragma once

#include "Component/Animation.hpp"
#include "GameObject.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/Debug.hpp"
#include "imgui.h"
#include "System/TimeManager.hpp"
#include "System/ResourcesManager.hpp"
#include "Rendering/Texture.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

	
Animation::Animation(GameObject* _gameObject, std::string _animationPath, Mesh* _mesh):
	Component(_gameObject)
{
	Assimp::Importer importer;

    _animationPath = "resources/dancing_vampire.dae";

    const aiScene* scene = importer.ReadFile(_animationPath, aiProcess_Triangulate);

    //ZASSERT(scene && scene->mRootNode, "Cant read animation file");

    auto animation = scene->mAnimations[0];
    duration = (float)animation->mDuration;
    tickPerSecond = (int)animation->mTicksPerSecond;

    animationShader = *ResourcesManager::GetResource<Shader>("AnimShader");

    mesh = *ResourcesManager::GetResource<Mesh>("Vampire");
    boneInfoMap = mesh->boneInfoMap;

    finalBonesMatrices.reserve(100);
    for (int i = 0; i < 100; i++)
        finalBonesMatrices.push_back(Mat4::identity);

    ReadHeirarchyData(rootNode, scene->mRootNode);
    ReadMissingBones(animation);
}

void Animation::ReadMissingBones(const aiAnimation* animation)
{
    for (int i = 0; i < animation->mNumChannels; i++)
    {
        auto channel = animation->mChannels[i];

        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = mesh->boneCounter;
            mesh->boneCounter++;
        }
        bones.push_back(Bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }

    mesh->boneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
    dest.name = src->mName.data;
    dest.transformation = Mat4::ConvertAssimpMatrixToMat4(src->mTransformation);
    dest.childrenCount = src->mNumChildren;

    for (unsigned int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHeirarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

Bone* Animation::FindBone(const std::string& _boneName)
{
    auto iter = std::find_if(bones.begin(), bones.end(),
        [&](const Bone& Bone)
        {
            return Bone.name == _boneName;
        }
    );

    if (iter == bones.end())
    {
        //std::cout << "Bone not found" << _boneName << std::endl;
        return nullptr;
    }
    else
    {
        //std::cout << "Bone FOUND" << std::endl;
        return &(*iter);
    }
}

void Animation::Editor()
{
    if (ImGui::Button("Play"))
    {
        play = !play;
        currentTime = 0.0f;
    }
}

void Animation::UpdateAnimation()
{
    if (play)
    {
        currentTime += tickPerSecond * TimeManager::GetDeltaTime();
        currentTime = fmod(currentTime, duration);
        CalculateBoneTransform(&rootNode, Mat4::identity);
    }
}

void Animation::CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform)
{
    std::string nodeName = _node->name;
    Mat4 nodeTransform = _node->transformation;

    Bone* Bone = FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(currentTime);
        nodeTransform = Bone->localTransform;
    }

    Mat4 globalTransformation = _parentTransform * nodeTransform;

    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        Mat4 offset = boneInfoMap[nodeName].offset;
        
        //offset.Display();

        finalBonesMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < _node->childrenCount; i++)
        CalculateBoneTransform(&_node->children[i], globalTransformation);
}

void Animation::Draw(const Camera& _camera)
{
    UpdateAnimation();

    animationShader->Use();

    animationShader->SetMatrix("view", _camera.GetMatrix().Reversed());
    animationShader->SetMatrix("projection", _camera.GetProjetionMatrix());

    for (int i = 0; i < finalBonesMatrices.size(); ++i)
        animationShader->SetMatrix("finalBonesMatrices[" + std::to_string(i) + "]", finalBonesMatrices[i]);

    animationShader->SetMatrix("model", Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale()));

    std::shared_ptr<Texture> text = *ResourcesManager::GetResource<Texture>("Default");

    text->Use();
    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}