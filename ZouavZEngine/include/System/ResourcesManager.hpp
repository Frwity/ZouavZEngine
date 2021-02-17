#pragma once

#include "Resource.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include "System/Debug.hpp"

class ResourcesManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Resource>> resources;

public:
	ResourcesManager() = delete;

	template<typename T, typename... Args>
	static Resource* AddResource(std::string _name, Args... _args)
	{
		auto a = resources.emplace(_name, std::make_unique<T>(_args...));
		if (a.second)
		{
			Debug::Log("Resource : " + _name + " loaded" );
			return a.first->second.get();
		}
		Debug::LogError("Resource : " + _name + " not loaded");
		return nullptr;
	}

	static Resource* GetResource(std::string _name);
};