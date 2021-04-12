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
	static std::unordered_map<std::string, std::unique_ptr<Texture>> textureResources;
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderResources;

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
			Debug::Log("Sound resource : " + _name + " loaded");
			return a.first->second.get();
		}
		Debug::LogError("Sound resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename Mesh* AddResourceMesh(std::string _name, Args... _args)
	{
		auto a = meshResources.emplace(_name, std::make_unique<Mesh>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Mesh resource : " + _name + " loaded");
			return a.first->second.get();
		}
		Debug::LogError("Mesh resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename Texture* AddResourceTexture(std::string _name, Args... _args)
	{
		auto a = textureResources.emplace(_name, std::make_unique<Texture>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Texture resource : " + _name + " loaded");
			return a.first->second.get();
		}
		Debug::LogError("Texture resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename Shader* AddResourceShader(std::string _name, Args... _args)
	{
		auto a = shaderResources.emplace(_name, std::make_unique<Shader>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Shader resource : " + _name + " loaded");
			return a.first->second.get();
		}
		Debug::LogError("Shader resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename T>
	static T* GetResource(std::string _name)
	{
		Debug::LogWarning("Wrong GetResource()");
	}

	template<>
	static Sound* GetResource<Sound>(std::string _name)
	{
		if (soundResources.find(_name) != soundResources.end())
			return soundResources.at(_name).get();
		else
			Debug::LogError("Sound resource : " + _name + " not found");	
	}

	template<>
	static Mesh* GetResource<Mesh>(std::string _name)
	{
		if (meshResources.find(_name) != meshResources.end())
			return meshResources.at(_name).get();
		else
			Debug::LogError("Mesh resource : " + _name + " not found");
	}

	template<>
	static Texture* GetResource<Texture>(std::string _name)
	{
		if (textureResources.find(_name) != textureResources.end())
			return textureResources.at(_name).get();
		else
			Debug::LogError("Texture resource : " + _name + " not found");
	}

	template<>
	static Shader* GetResource<Shader>(std::string _name)
	{
		if (shaderResources.find(_name) != shaderResources.end())
			return shaderResources.at(_name).get();
		else
			Debug::LogError("Shader resource : " + _name + " not found");
	}

	template<typename T>
	static const std::unordered_map<std::string, std::unique_ptr<T>>& GetResources()
	{
		Debug::LogWarning("Wrong GetResources()");
	}

	template<>
	static const std::unordered_map<std::string, std::unique_ptr<Texture>>& GetResources<Texture>()
	{
		return textureResources;
	}

	template<>
	static const std::unordered_map<std::string, std::unique_ptr<Mesh>>& GetResources<Mesh>()
	{
		return meshResources;
	}

	template<>
	static const std::unordered_map<std::string, std::unique_ptr<Shader>>& GetResources<Shader>()
	{
		return shaderResources;
	}

	template<>
	static const std::unordered_map<std::string, std::unique_ptr<Sound>>& GetResources<Sound>()
	{
		return soundResources;
	}

	template<typename T>
	static bool ResourceChanger(const char* _label, T*& _resource)
	{
		const std::unordered_map<std::string, std::unique_ptr<Resource>>* resources = reinterpret_cast<const std::unordered_map<std::string, std::unique_ptr<Resource>>*>(&GetResources<T>());
		bool changed = false;
		_resource = (T*)ResourceChangerImpl(_label, _resource, *resources, changed);
		return changed;
	}

	static Resource* ResourceChangerImpl(const char* _label, Resource* _resource, const std::unordered_map<std::string, std::unique_ptr<Resource>>& _resources, bool& _changed);
};