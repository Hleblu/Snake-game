#pragma once
#include <string>

class PathUtils
{
public:
	static std::string getSaveFilePath(const std::string& gameName, const std::string& fileName);
};