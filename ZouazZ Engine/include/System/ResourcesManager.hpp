#pragma once

#include "Resource.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

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
			std::cout << "Resource : " + _name + " loaded" << std::endl;
			return a.first->second.get();
		}

		std::cout << "Resource : " + _name + " not loaded" << std::endl;
		return nullptr;
	}

	static Resource* GetResource(std::string _name);
};