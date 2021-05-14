#include "System/ResourcesManager.hpp"
#include "imgui.h"

std::unordered_map<std::string, std::shared_ptr<Sound>> ResourcesManager::soundResources;
std::unordered_map<std::string, std::shared_ptr<Mesh>> ResourcesManager::meshResources;
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourcesManager::textureResources;
std::unordered_map<std::string, std::shared_ptr<Shader>> ResourcesManager::shaderResources;
std::unordered_map<std::string, std::shared_ptr<Font>> ResourcesManager::fontResources;

void ResourcesManager::InitDefaultResources()
{
    ResourcesManager::AddResourceMesh("Default", false, "resources/Cube.obj");
    ResourcesManager::AddResourceShader("Default", false, "resources/BlinnPhongShader.vs", "resources/BlinnPhongShader.fs");
    ResourcesManager::AddResourceTexture("Default", false, "resources/white.png");
    ResourcesManager::AddResourceFont("Default", false, "resources/Arial.fnt");
}

void ResourcesManager::Clear()
{
    soundResources.clear();
    meshResources.clear();
    textureResources.clear();
    shaderResources.clear();
    fontResources.clear();
}

std::shared_ptr<Resource>& ResourcesManager::ResourceChangerImpl(const char* _label, std::shared_ptr<Resource>& _resource, const std::unordered_map<std::string, std::shared_ptr<Resource>>& _resources, bool& _changed)
{
    if (ImGui::BeginCombo(_label, _resource ? _resource->GetName().c_str() : ""))
    {
        for (auto& resource : _resources)
        {
            if (ImGui::Selectable(resource.first.c_str(), resource.second == _resource) && _resource != resource.second)
            {
                _resource = resource.second;
                _changed = true;
            }
        }
        ImGui::EndCombo();
    }
    return _resource;
}
