/*#include "Rendering/AnimResource.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/ResourcesManager.hpp"

AnimResource::AnimResource(const std::string& _name, std::string& _path)
	:Resource(_name)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate);

	auto animation = scene->mAnimations[0];
	duration = (float)animation->mDuration;
	tickPerSecond = (int)animation->mTicksPerSecond;

	animationShader = *ResourcesManager::GetResource<Shader>("AnimShader");
}*/