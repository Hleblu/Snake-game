#pragma once
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

class PathUtils
{
public:
	static std::string getSaveFilePath(const std::string& gameName, const std::string& fileName)
	{
		fs::path saveDir;

#ifdef _WIN32
		if (const char* appData = std::getenv("APPDATA"))
		{
			saveDir = fs::path(appData) / gameName;
		}
		else {
			saveDir = fs::current_path() / saveDir;
		}
#else
		if (const char* home = std::getenv("HOME"))
		{
			saveDir = fs::path(home) / ("." + gameName);
		}
		else {
			saveDir = fs::current_path() / saveDir;
		}
#endif

		if (!fs::exists(saveDir))
			fs::create_directories(saveDir);

		return (saveDir / fileName).string();
	}
};