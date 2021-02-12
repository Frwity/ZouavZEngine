#include "System/ResourcesManager.hpp"

std::unordered_map<std::string, std::unique_ptr<Resource>> ResourcesManager::resources;

Resource* ResourcesManager::GetResource(std::string _name)
{
	return resources.at(_name).get();
}