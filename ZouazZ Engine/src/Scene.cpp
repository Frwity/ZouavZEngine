#include "GameObject.hpp"
#include "Scene.hpp"
#include "Rendering/MeshRenderer.hpp"
#include "Maths/Mat4.hpp"

GameObject& Scene::AddGameObject()
{
	children.push_back(GameObject());
	return children.back();
}

void Scene::Draw()
{
	for(const GameObject& child : children)
	{
		child.GetComponent<MeshRenderer>()->Draw(Mat4::Identity());
	}
}