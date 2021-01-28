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

    shader->SetMatrix("view", _camera.GetMatrix());
    shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    shader->SetMatrix("model", _parent * Mat4::CreateTRSMatrix( gameObject->transform.position,
                                                                gameObject->transform.rotation, 
                                                                gameObject->transform.scale));
    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);

}