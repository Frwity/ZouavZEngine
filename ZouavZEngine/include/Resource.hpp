#pragma once

#include <string>
#include <vector>

class Resource
{
protected:
	friend class ResourcesManager;
	std::string name;
	std::vector<std::string> paths;
	int nbUse = 0;
public:

	Resource() = default;
	Resource(const std::string& _name) : name(_name) {}
	virtual ~Resource() = default;

	int NbUse() { return nbUse; }
	void AddUse() { nbUse++; }
	void RemoveUse() { nbUse--; }
	const std::string& GetName() { return name; }
};