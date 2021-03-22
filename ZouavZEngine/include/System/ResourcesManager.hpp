#pragma once

#include "Resource.hpp"
#include "Sound.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Shader.hpp"
#include <unordered_map>

#include <memory>
#include <string>
#include <iostream>
#include "System/Debug.hpp"

class ResourcesManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Sound>> soundResources;
	static std::unordered_map<std::string, std::unique_ptr<Mesh>> meshResources;
	static std::vector<const char*> meshNames;
	static bool isMeshesDirty;
	static std::unordered_map<std::string, std::unique_ptr<Texture>> textureResources;
	static std::vector<const char*> textureNames; 
	static bool isTexturesDirty;
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderResources;
	static std::vector<const char*> shaderNames;
	static bool isShadersDirty;

public:
	ResourcesManager() = delete;

	//template<typename T, typename... Args>
	//static T* AddResource(std::string _name, Args... _args)
	//{
	//	Debug::Log("Wrong Resource Type");
	//	return nullptr;
	//}

	template<typename... Args>
	static typename Sound* AddResourceSound(std::string _name, Args... _args)
	{
		auto a = soundResources.emplace(_name, std::make_unique<Sound>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Sound Resource : " + _name + " loaded");
			return a.first->second.get();
		}
		Debug::LogError("Sound Resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename Mesh* AddResourceMesh(std::string _name, Args... _args)
	{
		auto a = meshResources.emplace(_name, std::make_unique<Mesh>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Mesh Resource : " + _name + " loaded");
			isMeshesDirty = true;
			return a.first->second.get();
		}
		Debug::LogError("Mesh Resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename Texture* AddResourceTexture(std::string _name, Args... _args)
	{
		auto a = textureResources.emplace(_name, std::make_unique<Texture>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Texture Resource : " + _name + " loaded");
			isTexturesDirty = true;
			return a.first->second.get();
		}
		Debug::LogError("Texture Resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename Shader* AddResourceShader(std::string _name, Args... _args)
	{
		auto a = shaderResources.emplace(_name, std::make_unique<Shader>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Shader Resource : " + _name + " loaded");
			isShadersDirty = true;
			return a.first->second.get();
		}
		Debug::LogError("Shader Resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename T>
	static T* GetResource(std::string _name)
	{
		Debug::LogWarning("Wrong get resource");
	}

	template<>
	static Sound* GetResource<Sound>(std::string _name)
	{
		return soundResources.at(_name).get();
	}

	template<>
	static Mesh* GetResource<Mesh>(std::string _name)
	{
		return meshResources.at(_name).get();
	}

	template<>
	static Texture* GetResource<Texture>(std::string _name)
	{
		return textureResources.at(_name).get();
	}

	template<>
	static Shader* GetResource<Shader>(std::string _name)
	{
		return shaderResources.at(_name).get();
	}

	template<typename T>
	static const std::vector<const char*>& GetResourceNames()
	{
		Debug::LogWarning("Wrong get resources");
	}

	template<>
	static const std::vector<const char*>& GetResourceNames<Texture>()
	{
		if (isTexturesDirty)
		{
			textureNames.clear();
			for (const auto& texture : textureResources)
			{
				textureNames.push_back(texture.first.c_str());
			}
			isTexturesDirty = false;
		}

		return textureNames;
	}

	template<>
	static const std::vector<const char*>& GetResourceNames<Mesh>()
	{
		if (isMeshesDirty)
		{
			meshNames.clear();
			for (const auto& mesh : meshResources)
			{
				meshNames.push_back(mesh.first.c_str());
			}
			isMeshesDirty = false;
		}

		return meshNames;
	}

	template<>
	static const std::vector<const char*>& GetResourceNames<Shader>()
	{
		if (isShadersDirty)
		{
			shaderNames.clear();
			for (const auto& shader : shaderResources)
			{
				shaderNames.push_back(shader.first.c_str());
			}
			isShadersDirty = false;
		}

		return shaderNames;
	}

	template<typename T>
	static int GetIndexByName(std::string _name)
	{
		Debug::LogWarning("Wrong GetIndexByName");
	}

	template<>
	static int GetIndexByName<Texture>(std::string _name)
	{
		int cnt = 0;
		for (auto& texture : textureResources)
		{
			if (texture.first == _name)
				return cnt;
			cnt++;
		}

		return -1;
	}

	template<>
	static int GetIndexByName<Mesh>(std::string _name)
	{
		int cnt = 0;
		for (auto& mesh : meshResources)
		{
			if (mesh.first == _name)
				return cnt;
			cnt++;
		}

		return -1;
	}

	template<>
	static int GetIndexByName<Shader>(std::string _name)
	{
		int cnt = 0;
		for (auto& shader : shaderResources)
		{
			if (shader.first == _name)
				return cnt;
			cnt++;
		}

		return -1;
	}
};