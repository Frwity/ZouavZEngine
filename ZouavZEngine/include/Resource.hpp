#pragma once

#include <string>
#include <vector>

class Resource
{
protected:
	friend class ResourcesManager;
	std::string name;
	std::vector<std::string> paths;
	bool deletable = true;

public:

	Resource() = default;
	Resource(const std::string& _name) : name(_name) {}
	virtual ~Resource() = default;

	virtual void RemoveFromResourcesManager() = 0;

	bool IsDeletable() { return deletable; }

	const std::string& GetName() { return name; }
};