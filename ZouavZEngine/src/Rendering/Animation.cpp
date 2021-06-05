#pragma once

#include "Component/Animation.hpp"
#include "GameObject.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/Debug.hpp"
#include "imgui.h"
#include "System/TimeManager.hpp"
#include "System/ResourcesManager.hpp"
#include "Component/MeshRenderer.hpp"
#include "Rendering/Texture.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

	
Animation::Animation(GameObject* _gameObject, std::string _animationPath, Mesh* _mesh, std::string _name):
	Component(_gameObject, _name)
{
    currentAnimation = nullptr;
    if (gameObject->GetComponent<MeshRenderer>())
    {
        mesh = gameObject->GetComponent<MeshRenderer>()->mesh.get();
        text = gameObject->GetComponent<MeshRenderer>()->material.texture.get();
        color = gameObject->GetComponent<MeshRenderer>()->material.color;
    }

    animationShader = *ResourcesManager::GetResource<Shader>("AnimShader");
}

void Animation::Editor()
{
    if (ResourcesManager::ResourceChanger<AnimResource>("Animations", currentAnimation))
    {
        if(currentAnimation)
            currentAnimation->UpdateAnimationResources(mesh);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new animation");

            std::string _path = *(const std::string*)payload->Data;
            std::string _truePath = _path;
            size_t start_pos = _truePath.find("\\");
            _truePath.replace(start_pos, 1, "/");

            if (_truePath.find(".fbx") != std::string::npos || _truePath.find(".dae") != std::string::npos)
            {
                currentAnimation = *ResourcesManager::AddResourceAnimation(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath, mesh);
                if (currentAnimation)
                {
                    currentAnimation->UpdateAnimationResources(mesh);
                    animationsAttached.insert(std::make_pair(currentAnimation->GetName(), currentAnimation));
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if(currentAnimation)
        ImGui::InputFloat("AnimationDuration", &currentAnimation->animationSpeed);

    if (ImGui::Button("Play"))
    {
        play = !play;

        if(currentAnimation)
            currentAnimation->currentTime = 0.0f;
    }
}

void Animation::Play(std::string _animName)
{
    currentAnimation = animationsAttached.find(_animName)->second;
    play = true;
}

bool Animation::IsPlaying(std::string _animName)
{
    return currentAnimation->GetName().compare(_animName) == 0 && IsPlaying();
}

void Animation::Draw(const Camera& _camera)
{
    if (play && currentAnimation)
        currentAnimation->UpdateAnimation(TimeManager::GetDeltaTime(), loop);
    else
        return;

    if(!mesh)
        return;

    animationShader->Use();

    animationShader->SetMatrix("view", _camera.GetMatrix().Reversed());
    animationShader->SetMatrix("projection", _camera.GetProjectionMatrix());
    animationShader->SetVector4("color", color);

    for (int i = 0; i < currentAnimation->finalBonesMatrices.size(); ++i)
        animationShader->SetMatrix("finalBonesMatrices[" + std::to_string(i) + "]", currentAnimation->finalBonesMatrices[i]);
    
    //temp fix mesh scale
    animationShader->SetMatrix("model", Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale() / 100));

    text->Use();
    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}

template <class Archive>
static void Animation::load_and_construct(Archive& _ar, cereal::construct<Animation>& _construct)
{
    std::string meshName;
    int animAttachSize = 0;
    std::vector<std::string> animNames;
    std::vector<std::string> animPaths;
    
    _ar(animAttachSize);
    for (int i = 0; i < animAttachSize; i++)
    {
        std::string name;
        std::string path;
        _ar(name);
        _ar(path);
        animNames.push_back(name);
        animPaths.push_back(path);
    }

    _construct(GameObject::currentLoadedGameObject);
    _ar(cereal::base_class<Component>(_construct.ptr()));
    for (int i = 0; i < animAttachSize; i++)
    {
        _construct->animationsAttached.insert(std::make_pair(animNames[i], *ResourcesManager::AddResourceAnimation(animNames[i], true, animPaths[i])));
    }
}