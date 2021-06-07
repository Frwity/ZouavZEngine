#include "Rendering/AnimResource.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/ResourcesManager.hpp"

AnimResource::AnimResource(const std::string& _name, std::string& _path, Mesh* _mesh)
	:Resource(_name, _path.c_str())
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene)
	{
		Debug::LogWarning(std::string("Animation load failed!: ").append(_path));
		return;
	}

	auto animation = scene->mAnimations[0];
	duration = (float)animation->mDuration;
	tickPerSecond = (int)animation->mTicksPerSecond;

	mesh = _mesh;

	if (!mesh)
		return;

	boneInfoMap = mesh->boneInfoMap;

	finalBonesMatrices.reserve(100 * 16);
	Mat4 mat = Mat4::identity;
	for (int i = 0; i < 100; i++)
	{
		finalBonesMatrices.emplace_back(mat.matrix[0]);
		finalBonesMatrices.emplace_back(mat.matrix[1]);
		finalBonesMatrices.emplace_back(mat.matrix[2]);
		finalBonesMatrices.emplace_back(mat.matrix[3]);
		finalBonesMatrices.emplace_back(mat.matrix[4]);
		finalBonesMatrices.emplace_back(mat.matrix[5]);
		finalBonesMatrices.emplace_back(mat.matrix[6]);
		finalBonesMatrices.emplace_back(mat.matrix[7]);
		finalBonesMatrices.emplace_back(mat.matrix[8]);
		finalBonesMatrices.emplace_back(mat.matrix[9]);
		finalBonesMatrices.emplace_back(mat.matrix[10]);
		finalBonesMatrices.emplace_back(mat.matrix[11]);
		finalBonesMatrices.emplace_back(mat.matrix[12]);
		finalBonesMatrices.emplace_back(mat.matrix[13]);
		finalBonesMatrices.emplace_back(mat.matrix[14]);
		finalBonesMatrices.emplace_back(mat.matrix[15]);
	}

	ReadHeirarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation);
	Debug::Log("Animation resource : " + _name + " loaded");
}

void AnimResource::UpdateAnimationResources(Mesh* _mesh)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene)
	{
		Debug::LogWarning(std::string("Animation load failed!: ").append(path));
		return;
	}
	auto animation = scene->mAnimations[0];
	duration = (float)animation->mDuration;
	tickPerSecond = (int)animation->mTicksPerSecond;

	mesh = _mesh;

	boneInfoMap = mesh->boneInfoMap;

	finalBonesMatrices.clear();
	finalBonesMatrices.reserve(100 * 16);
	Mat4 mat = Mat4::identity;
	for (int i = 0; i < 100; i++)
	{
		finalBonesMatrices.emplace_back(mat.matrix[0]);
		finalBonesMatrices.emplace_back(mat.matrix[1]);
		finalBonesMatrices.emplace_back(mat.matrix[2]);
		finalBonesMatrices.emplace_back(mat.matrix[3]);
		finalBonesMatrices.emplace_back(mat.matrix[4]);
		finalBonesMatrices.emplace_back(mat.matrix[5]);
		finalBonesMatrices.emplace_back(mat.matrix[6]);
		finalBonesMatrices.emplace_back(mat.matrix[7]);
		finalBonesMatrices.emplace_back(mat.matrix[8]);
		finalBonesMatrices.emplace_back(mat.matrix[9]);
		finalBonesMatrices.emplace_back(mat.matrix[10]);
		finalBonesMatrices.emplace_back(mat.matrix[11]);
		finalBonesMatrices.emplace_back(mat.matrix[12]);
		finalBonesMatrices.emplace_back(mat.matrix[13]);
		finalBonesMatrices.emplace_back(mat.matrix[14]);
		finalBonesMatrices.emplace_back(mat.matrix[15]);
	}

	ReadHeirarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation);
}

void AnimResource::RemoveFromResourcesManager()
{

}

void AnimResource::UpdateAnimation(float _deltaTime, bool _loop, float& _currentTime, bool& _animationFinish)
{
	if (!_animationFinish)
	{
		_currentTime += tickPerSecond * animationSpeed * _deltaTime;

		if (_currentTime >= duration)
		{
			_currentTime = 0.0f;
			_animationFinish = true;
		}
	}
	if (_animationFinish && _loop)
		_animationFinish = false;

	if(!_animationFinish)
		CalculateBoneTransform(&rootNode, Mat4::identity, _currentTime);	
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

void AnimResource::CalculateBoneTransform(const AssimpNodeData* _node, Mat4 _parentTransform, float _currentTime)
{
	std::string nodeName = _node->name;
	Mat4 nodeTransform = _node->transformation;

	Bone* Bone = FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(_currentTime);
		nodeTransform = Bone->localTransform;
	}

	Mat4 globalTransformation = _parentTransform * nodeTransform;

	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		Mat4 offset = boneInfoMap[nodeName].offset;

		Mat4 mat = globalTransformation * offset;
		finalBonesMatrices[index * 16] = mat.matrix[0];
		finalBonesMatrices[index * 16 + 1] = mat.matrix[1];
		finalBonesMatrices[ index * 16 + 2] = mat.matrix[2];
		finalBonesMatrices[ index * 16 + 3] = mat.matrix[3];
		finalBonesMatrices[ index * 16 + 4] = mat.matrix[4];
		finalBonesMatrices[ index * 16 + 5] = mat.matrix[5];
		finalBonesMatrices[ index * 16 + 6] = mat.matrix[6];
		finalBonesMatrices[ index * 16 + 7] = mat.matrix[7];
		finalBonesMatrices[ index * 16 + 8] = mat.matrix[8];
		finalBonesMatrices[ index * 16 + 9] = mat.matrix[9];
		finalBonesMatrices[ index * 16 + 10] = mat.matrix[10];
		finalBonesMatrices[ index * 16 + 11] = mat.matrix[11];
		finalBonesMatrices[ index * 16 + 12] = mat.matrix[12];
		finalBonesMatrices[ index * 16 + 13] = mat.matrix[13];
		finalBonesMatrices[ index * 16 + 14] = mat.matrix[14];
		finalBonesMatrices[ index * 16 + 15] = mat.matrix[15];
	}

	for (int i = 0; i < _node->childrenCount; i++)
		CalculateBoneTransform(&_node->children[i], globalTransformation, _currentTime);
}