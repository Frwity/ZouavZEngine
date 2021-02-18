#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "System/ResourcesManager.hpp"
#include "Component/MeshRenderer.hpp"
#include "System/Debug.hpp"

MeshRenderer::MeshRenderer(GameObject* _gameObject, Mesh* _mesh, Shader* _shader, Texture* _texture)
	: Component(_gameObject), mesh{ _mesh }, shader{ _shader }, texture{ _texture }
{}

MeshRenderer::MeshRenderer(GameObject* _gameObject)
    : Component(_gameObject), 
      mesh{ static_cast<Mesh*>(ResourcesManager::GetResource("Skull Mesh")) }, 
      shader{static_cast<Shader*>(ResourcesManager::GetResource("BlinnPhongShader")) }, 
      texture{ static_cast<Texture*>(ResourcesManager::GetResource("Skull Tex")) }
{}

void MeshRenderer::Draw(const Mat4& heritedMatrix, const Camera& _camera)
{
    shader->Use();
    glActiveTexture(GL_TEXTURE0);
    texture->Use();
    Mat4 matrixCamera = _camera.GetMatrix();

    shader->SetMatrix("view", matrixCamera.Reversed());
    shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    shader->SetMatrix("model", heritedMatrix * Mat4::CreateTRSMatrix(gameObject->position, gameObject->rotation, gameObject->scale));
    shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));

    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);

}