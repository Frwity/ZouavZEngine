#include "Resource.hpp"
#include "imgui.h"
#include "System/ResourcesManager.hpp"

template<typename T>
void Resource::ResourceChanger(const char* _label, T*& _resource)
{
    const auto& resources = ResourcesManager::GetResources<T>();

    if (ImGui::BeginCombo(_label, _resource ? _resource->GetName().c_str() : ""))
    {
        for (auto& resource : resources)
        {
            if (ImGui::Selectable(resource.first.c_str(), resource.second.get() == _resource))
                _resource = resource.second.get();
        }
        ImGui::EndCombo();
    }
}