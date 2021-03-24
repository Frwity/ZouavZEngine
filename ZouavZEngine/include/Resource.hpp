#pragma once

#include <string>

class Resource
{
private:
	std::string name;

public:

	Resource() = default;
	Resource(const std::string& _name) : name(_name) {}
	~Resource() = default;

	const std::string& GetName() { return name; }

	template<typename T>
	static void ResourceChanger(const char* _label, T*& _resource);
};