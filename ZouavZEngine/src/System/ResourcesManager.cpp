#include "System/ResourcesManager.hpp"

std::unordered_map<std::string, std::unique_ptr<Sound>> ResourcesManager::soundResources;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourcesManager::meshResources;
std::unordered_map<std::string, std::unique_ptr<Texture>> ResourcesManager::textureResources;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourcesManager::shaderResources;