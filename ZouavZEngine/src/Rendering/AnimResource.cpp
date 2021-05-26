#include "Rendering/AnimResource.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/ResourcesManager.hpp"

AnimResource::AnimResource(const std::string& _name, std::string& _path, Mesh* _mesh)
	:Resource(_name)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(_path, aiProcess_Triangulate);

	auto animation = scene->mAnimations[0];
	duration = (float)animation->mDuration;
	tickPerSecond = (int)animation->mTicksPerSecond;

	mesh = _mesh;

	animationShader = *ResourcesManager::GetResource<Shader>("AnimShader");

	boneInfoMap = mesh->boneInfoMap;

	finalBonesMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		finalBonesMatrices.push_back(Mat4::identity);

	ReadHeirarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation);

	paths.emplace_back(_path);
}

void AnimResource::RemoveFromResourcesManager()
{

}

void AnimResource::UpdateAnimation(float _deltaTime)
{
	currentTime += tickPerSecond * _deltaTime;
	currentTime = fmod(currentTime, duration);
	CalculateBoneTransform(&rootNode, Mat4::identity);
}

void AnimResource::ReadMissingBones(const aiAnimation* animation)
{
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		auto channel = animation->mChannels[i];

		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = mesh->boneCounter;
			mesh->boneCounter++;
		}
		bones.push_back(Bone(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id, channel));
	}

	mesh->boneInfoMap = boneInfoMap;
}

void AnimResource::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	dest.name = src->mName.data;
	dest.transformation = Mat4::ConvertAssimpMatrixToMat4(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

Bone* AnimResource::FindBone(const std::string& _boneName)
{
	auto iter = std::find_if(bones.begin(), bones.end(),
		[&](const Bone& Bone)
		{
			return Bone.name == _boneName;
		}
	);

	if (iter == bones.end())

		return nullptr;
	else
		return &(*iter);
}

void AnimResource::CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform)
{
	std::string nodeName = _node->name;
	Mat4 nodeTransform = _node->transformation;

	Bone* Bone = FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(currentTime);
		nodeTransform = Bone->localTransform;
	}

	Mat4 globalTransformation = _parentTransform * nodeTransform;

	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		Mat4 offset = boneInfoMap[nodeName].offset;

		finalBonesMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < _node->childrenCount; i++)
		CalculateBoneTransform(&_node->children[i], globalTransformation);
}