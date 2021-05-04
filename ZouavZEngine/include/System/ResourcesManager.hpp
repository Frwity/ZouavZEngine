#pragma once

#include "Resource.hpp"
#include "Sound.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Shader.hpp"
#include <unordered_map>
#include <cstdarg>
#include <memory>
#include <string>
#include <iostream>
#include "System/Debug.hpp"

#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"

class ResourcesManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Sound>> soundResources;
	static std::unordered_map<std::string, std::unique_ptr<Mesh>> meshResources;
	static std::unordered_map<std::string, std::unique_ptr<Texture>> textureResources;
	static std::unordered_map<std::string, std::unique_ptr<Shader>> shaderResources;

public:
	ResourcesManager() = delete;

	static void InitDefaultResources();
	static void Clear();

	template <class Archive>
	static void load(Archive& _ar)
	{
		AddResourceMesh("Default")->CreateCube();
		int count;
		std::string name;
		std::string path1;
		std::string path2;
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, path1);
			AddResourceSound(name, path1.c_str());
		}
		_ar(count);
		for (int i = 0; i < count; ++i) // TODO cube mesh 
		{
			_ar(name, path1);
			if (i == 0)
				continue;
			AddResourceMesh(name, path1.c_str());
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, path1);
			AddResourceTexture(name, path1.c_str());
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, path1, path2);
			AddResourceShader(name, path1.c_str(), path2.c_str());
		}
	}

	template <class Archive>
	static void save(Archive& _ar)
	{
		_ar(soundResources.size());
		for (auto& sound : soundResources)
			_ar(sound.first, sound.second.get()->paths[0]);
		
		_ar(meshResources.size());
		for (auto& mesh : meshResources)
			_ar(mesh.first, mesh.second.get()->paths[0]);
		
		_ar(textureResources.size());
		for (auto& texture : textureResources)
			_ar(texture.first, texture.second.get()->paths[0]);
		
		_ar(shaderResources.size());
		for (auto& shader : shaderResources)
			_ar(shader.first, shader.second.get()->paths[0], shader.second.get()->paths[1]);
	}

	template<typename... Args>
	static typename Sound* AddResourceSound(std::string _name, Args... _args)
	{
		auto a = soundResources.emplace(_name, std::make_unique<Sound>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Sound resource : " + _name + " loaded");
			return a.first->second.get();
		}
		else if (a.first->second.get())
		{
			a.first->second.get()->AddUse();
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
		else if (a.first->second.get())
		{
			a.first->second.get()->AddUse();
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
		else if (a.first->second.get())
		{
			a.first->second.get()->AddUse();
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
		else if (a.first->second.get())
		{
			a.first->second.get()->AddUse();
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
		return nullptr;
	}

	template<>
	static Mesh* GetResource<Mesh>(std::string _name)
	{
		if (meshResources.find(_name) != meshResources.end())
			return meshResources.at(_name).get();
		else
			Debug::LogError("Mesh resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static Texture* GetResource<Texture>(std::string _name)
	{
		if (textureResources.find(_name) != textureResources.end())
			return textureResources.at(_name).get();
		else
			Debug::LogError("Texture resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static Shader* GetResource<Shader>(std::string _name)
	{
		if (shaderResources.find(_name) != shaderResources.end())
			return shaderResources.at(_name).get();
		else
			Debug::LogError("Shader resource : " + _name + " not found");
		return nullptr;
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

	static void RemoveResourceSound(std::string _name)
	{
		soundResources.erase(_name);
	}

	static void RemoveResourceTexture(std::string _name)
	{
		textureResources.erase(_name);
	}

	static void RemoveResourceMesh(std::string _name)
	{
		meshResources.erase(_name);
	}

	static void RemoveResourceShader(std::string _name)
	{
		shaderResources.erase(_name);
	}

	template<typename T>
	static bool ResourceChanger(const char* _label, T*& _resource)
	{
		const std::unordered_map<std::string, std::unique_ptr<Resource>>* resources = reinterpret_cast<const std::unordered_map<std::string, std::unique_ptr<Resource>>*>(&GetResources<T>());
		bool changed = false;
		T* tempResource = (T*)ResourceChangerImpl(_label, _resource, *resources, changed);
		//if (changed)
		//{
		//	_resource->RemoveUse();
		//	if (_resource->NbUse() <= 0)
		//	{
		//		if (dynamic_cast<Texture*>(_resource))
		//			ResourcesManager::RemoveResourceTexture(_resource->GetName());
		//		else if (dynamic_cast<Shader*>(_resource))
		//			ResourcesManager::RemoveResourceShader(_resource->GetName());
		//		else if (dynamic_cast<Mesh*>(_resource))
		//			ResourcesManager::RemoveResourceMesh(_resource->GetName());
		//		else if (dynamic_cast<Sound*>(_resource))
		//			ResourcesManager::RemoveResourceSound(_resource->GetName());

		//	}
		//}
		_resource = tempResource;
		return changed;
	}

	static Resource* ResourceChangerImpl(const char* _label, Resource* _resource, const std::unordered_map<std::string, std::unique_ptr<Resource>>& _resources, bool& _changed);
};