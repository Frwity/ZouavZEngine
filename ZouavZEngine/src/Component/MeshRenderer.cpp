#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "System/ResourcesManager.hpp"
#include "Component/MeshRenderer.hpp"
#include "System/Debug.hpp"
#include "imgui.h"

MeshRenderer::MeshRenderer(GameObject* _gameObject, Mesh* _mesh, Texture* _texture, Shader* _shader)
    : Component(_gameObject), mesh{ _mesh }, material{ _shader, _texture, {1.0f, 1.0f, 1.0f, 1.0f} }
{}

MeshRenderer::MeshRenderer(GameObject* _gameObject)
    : Component(_gameObject), 
      mesh{ ResourcesManager::GetResource<Mesh>("Default") }
{}

void MeshRenderer::Draw(const Mat4& heritedMatrix, const Camera& _camera)
{
    material.shader->Use();
    glActiveTexture(GL_TEXTURE0);
    Texture::Use(material.texture);
    Mat4 matrixCamera = _camera.GetMatrix();

    material.shader->SetMatrix("view", matrixCamera.Reversed());
    material.shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    material.shader->SetMatrix("model", Mat4::CreateTRSMatrix(GetGameObject().WorldPosition(), GetGameObject().WorldRotation(), GetGameObject().WorldScale()));
    material.shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
    material.shader->SetVector4("color", material.color);

    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::Editor()
{
	ResourcesManager::ResourceChanger<Texture>("Texture", material.texture);
	ResourcesManager::ResourceChanger<Mesh>("Mesh", mesh);
	ResourcesManager::ResourceChanger<Shader>("Shader", material.shader);
	ImGui::ColorEdit4("Color : ", &material.color.w);
}