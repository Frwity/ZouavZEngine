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
    if (gameObject->GetComponent<MeshRenderer>())
    {
        mesh = gameObject->GetComponent<MeshRenderer>()->mesh.get();
        text = gameObject->GetComponent<MeshRenderer>()->material.texture.get();
    }

    animationShader = *ResourcesManager::GetResource<Shader>("AnimShader");
}

void Animation::Editor()
{
    ResourcesManager::ResourceChanger<AnimResource>("Animation", currentAnimation);
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
                if (currentAnimation.use_count() == 2 && currentAnimation->IsDeletable())
                    ResourcesManager::RemoveResourceAnimation(currentAnimation->GetName());
                currentAnimation = *ResourcesManager::AddResourceAnimation(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath, mesh);
            }
        }

        ImGui::EndDragDropTarget();
    }

    if (ImGui::Button("Play"))
    {
        play = !play;
        currentAnimation->currentTime = 0.0f;
    }
}

void Animation::Draw(const Camera& _camera)
{
    if (play && currentAnimation)
        currentAnimation->UpdateAnimation(TimeManager::GetDeltaTime());

    animationShader->Use();

    animationShader->SetMatrix("view", _camera.GetMatrix().Reversed());
    animationShader->SetMatrix("projection", _camera.GetProjectionMatrix());

    for (int i = 0; i < currentAnimation->finalBonesMatrices.size(); ++i)
        animationShader->SetMatrix("finalBonesMatrices[" + std::to_string(i) + "]", currentAnimation->finalBonesMatrices[i]);

    animationShader->SetMatrix("model", Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale()));

    text->Use();
    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}

template <class Archive>
static void Animation::load_and_construct(Archive& _ar, cereal::construct<Animation>& _construct)
{
    _construct(GameObject::currentLoadedGameObject);
    _ar(cereal::base_class<Component>(_construct.ptr()));
}