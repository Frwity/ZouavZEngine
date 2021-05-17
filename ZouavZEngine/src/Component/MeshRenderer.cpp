#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "System/ResourcesManager.hpp"
#include "Component/MeshRenderer.hpp"
#include "System/Debug.hpp"
#include "imgui.h"

MeshRenderer::MeshRenderer(GameObject* _gameObject, std::shared_ptr<Mesh>& _mesh, std::shared_ptr<Texture>& _texture, std::shared_ptr<Shader>& _shader)
    : Component(_gameObject), mesh{ _mesh }, material{ _shader, _texture, {1.0f, 1.0f, 1.0f, 1.0f} }
{}

MeshRenderer::MeshRenderer(GameObject* _gameObject)
    : Component(_gameObject), 
      mesh{ *ResourcesManager::GetResource<Mesh>("Default") }
{}

MeshRenderer::~MeshRenderer()
{
    if (mesh.use_count() == 2 && mesh->IsDeletable())
        mesh->RemoveFromResourcesManager();
    if (material.texture.use_count() == 2 && material.texture->IsDeletable())
        material.texture->RemoveFromResourcesManager();
    if (material.shader.use_count() == 2 && material.shader->IsDeletable())
        material.shader->RemoveFromResourcesManager();
}

void MeshRenderer::Draw(const Camera& _camera)
{
    material.shader->Use();
    glActiveTexture(GL_TEXTURE0);
    Texture::Use(material.texture.get());
    //Mat4 matrixCamera = _camera.GetMatrix();

    //material.shader->SetMatrix("view", matrixCamera.Reversed());
    //material.shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    //material.shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
    material.shader->SetMatrix("model", Mat4::CreateTRSMatrix(GetGameObject().WorldPosition(), GetGameObject().WorldRotation(), GetGameObject().WorldScale()));
    material.shader->SetVector4("color", material.color);

    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::TextureEditor()
{
    ResourcesManager::ResourceChanger<Texture>("Texture", material.texture);
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new texture");
            std::string _path = *(const std::string*)payload->Data;
            std::string _truePath = _path;
            size_t start_pos = _truePath.find("\\");
            _truePath.replace(start_pos, 1, "/");

            if (_truePath.find(".png") != std::string::npos || _truePath.find(".jpg") != std::string::npos)
            {
                if (material.texture.use_count() == 2 && material.texture->IsDeletable())
                    ResourcesManager::RemoveResourceTexture(material.texture->GetName());
                material.texture = *ResourcesManager::AddResourceTexture(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath.c_str());
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void MeshRenderer::MeshEditor()
{
    ResourcesManager::ResourceChanger<Mesh>("Mesh", mesh);
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new mesh");
            std::string _path = *(const std::string*)payload->Data;
            std::string _truePath = _path;
            size_t start_pos = _truePath.find("\\");
            _truePath.replace(start_pos, 1, "/");

            if (_truePath.find(".fbx") != std::string::npos || _truePath.find(".obj") != std::string::npos)
            {
                if (mesh.use_count() == 2 && mesh->IsDeletable())
                    ResourcesManager::RemoveResourceMesh(mesh->GetName());
                mesh = *ResourcesManager::AddResourceMesh(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath.c_str());
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void MeshRenderer::ShaderEditor()
{
    ResourcesManager::ResourceChanger<Shader>("Shader", material.shader);
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new shader");
            std::string _path = *(const std::string*)payload->Data;
            std::string _truePath = _path;
            size_t start_pos = _truePath.find("\\");
            _truePath.replace(start_pos, 1, "/");

            if (_truePath.find(".shader") != std::string::npos)
            {
                if (material.shader.use_count() == 2 && material.shader->IsDeletable())
                    ResourcesManager::RemoveResourceShader(material.shader->GetName());
                material.shader = *ResourcesManager::AddResourceShader(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath.c_str());
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void MeshRenderer::Editor()
{
    TextureEditor();
    MeshEditor();
    ShaderEditor();
	ImGui::ColorEdit4("Color : ", &material.color.w);
}

template <class Archive>
static void MeshRenderer::load_and_construct(Archive& _ar, cereal::construct<MeshRenderer>& _construct)
{
	std::string meshName;
    std::string meshPath;
	std::string textureName;
    std::string texturePath;
	std::string shaderName;
    std::string shaderPath;
    bool isMeshDeletebale;
    bool isTextureDeletebale;
    bool isShaderDeletebale;
    _ar(meshName, isMeshDeletebale, meshPath, textureName, isTextureDeletebale, texturePath, shaderName, isShaderDeletebale, shaderPath);
	_construct(GameObject::currentLoadedGameObject, *ResourcesManager::AddResourceMesh(meshName, isMeshDeletebale, meshPath.c_str()),
		*ResourcesManager::AddResourceTexture(textureName, isTextureDeletebale, texturePath.c_str()),
		*ResourcesManager::AddResourceShader(shaderName, isShaderDeletebale, shaderPath.c_str()));
    _ar(cereal::base_class<Component>(_construct.ptr()));
}
