#include "System/ResourcesManager.hpp"
#include "imgui.h"

std::unordered_map<std::string, std::unique_ptr<Sound>> ResourcesManager::soundResources;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourcesManager::meshResources;
std::unordered_map<std::string, std::unique_ptr<Texture>> ResourcesManager::textureResources;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourcesManager::shaderResources;

void ResourcesManager::InitDefaultResources()
{
    ResourcesManager::AddResourceMesh("Default")->CreateCube();
    ResourcesManager::AddResourceShader("Default", "resources/BlinnPhongShader.vs", "resources/BlinnPhongShader.fs");
    ResourcesManager::AddResourceTexture("Default", "resources/white.png");
}

void ResourcesManager::Clear()
{
    soundResources.clear();
    meshResources.clear();
    textureResources.clear();
    shaderResources.clear();
}

Resource* ResourcesManager::ResourceChangerImpl(const char* _label, Resource* _resource, const std::unordered_map<std::string, std::unique_ptr<Resource>>& _resources, bool& _changed)
{
    if (ImGui::BeginCombo(_label, _resource ? _resource->GetName().c_str() : ""))
    {
        for (auto& resource : _resources)
        {
            if (ImGui::Selectable(resource.first.c_str(), resource.second.get() == _resource))
            {
                _resource = resource.second.get();
                _changed = true;
            }
        }
        ImGui::EndCombo();
    }
    return _resource;
}
