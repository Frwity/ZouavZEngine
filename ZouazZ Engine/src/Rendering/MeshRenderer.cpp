#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/MeshRenderer.hpp"

MeshRenderer::MeshRenderer(GameObject* _gameObject, Mesh* _mesh, Shader* _shader, Texture* _texture)
	: Component(_gameObject), mesh{ _mesh }, shader{ _shader }, texture{ _texture }
{}

void MeshRenderer::Draw(const Mat4& _parent, const Camera& _camera)
{
    shader->Use();
    glActiveTexture(GL_TEXTURE0);
    texture->Use();
    Mat4 matrixCamera = _camera.GetMatrix();

    shader->SetMatrix("view", matrixCamera.Reverse());
    shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    shader->SetMatrix("model", _parent * Mat4::CreateTRSMatrix( gameObject->transform.position,
                                                                gameObject->transform.rotation, 
                                                                gameObject->transform.scale));
    shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));

    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);

}