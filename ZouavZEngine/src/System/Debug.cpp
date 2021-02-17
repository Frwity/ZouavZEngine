#include "System/Debug.hpp"

std::vector<std::string> Debug::logs;
std::vector<std::string> Debug::errorLogs;

void Debug::Log(std::string log)
{
	logs.push_back(log);
}

void Debug::LogError(std::string errorLog)
{
	errorLogs.push_back(errorLog);
}