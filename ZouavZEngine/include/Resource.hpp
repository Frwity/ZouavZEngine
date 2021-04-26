#pragma once

#include <string>
#include <vector>

class Resource
{
protected:
	friend class ResourcesManager;
	std::string name;
	std::vector<std::string> paths;
public:

	Resource() = default;
	Resource(const std::string& _name) : name(_name) {}
	~Resource() = default;

	const std::string& GetName() { return name; }
};