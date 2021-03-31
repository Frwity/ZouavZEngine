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
	: Component(_gameObject), mesh{ _mesh }, texture{ _texture }, shader{ _shader }
{}

MeshRenderer::MeshRenderer(GameObject* _gameObject)
    : Component(_gameObject), 
      mesh{ ResourcesManager::GetResource<Mesh>("Skull Mesh") }, 
      texture{ ResourcesManager::GetResource<Texture>("Skull Tex") },
      shader{ResourcesManager::GetResource<Shader>("BlinnPhongShader") }
{}

void MeshRenderer::Draw(const Mat4& heritedMatrix, const Camera& _camera)
{
    shader->Use();
    glActiveTexture(GL_TEXTURE0);
    Texture::Use(texture);
    Mat4 matrixCamera = _camera.GetMatrix();

    shader->SetMatrix("view", matrixCamera.Reversed());
    shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    shader->SetMatrix("model", Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale()));
    shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));

    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::Editor()
{
    ImGui::Text("MeshRenderer");
    ResourcesManager::ResourceChanger<Texture>("Texture", texture);
    ResourcesManager::ResourceChanger<Mesh>("Mesh", mesh);
    ResourcesManager::ResourceChanger<Shader>("Shader", shader);
}