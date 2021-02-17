#pragma once
#include <string>
#include <vector>

class Debug
{
private:
public:
	Debug() = delete;
	~Debug() = delete;
	
	//TODO discuss if we use one or more logs vectors
	static std::vector<std::string> logs;
	static std::vector<std::string> errorLogs;
	

	static void Log(std::string log);
	static void LogError(std::string errorLog);
};