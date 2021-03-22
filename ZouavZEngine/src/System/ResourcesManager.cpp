#include "System/ResourcesManager.hpp"

std::unordered_map<std::string, std::unique_ptr<Sound>> ResourcesManager::soundResources;
std::unordered_map<std::string, std::unique_ptr<Mesh>> ResourcesManager::meshResources;
std::vector<const char*> ResourcesManager::meshNames;
bool ResourcesManager::isMeshesDirty = true;
std::unordered_map<std::string, std::unique_ptr<Texture>> ResourcesManager::textureResources;
std::vector<const char*> ResourcesManager::textureNames;
bool ResourcesManager::isTexturesDirty = true;
std::unordered_map<std::string, std::unique_ptr<Shader>> ResourcesManager::shaderResources;
std::vector<const char*> ResourcesManager::shaderNames;
bool ResourcesManager::isShadersDirty = true;

