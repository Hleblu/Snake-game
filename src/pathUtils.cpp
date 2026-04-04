#include "pathUtils.hpp"
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

std::string PathUtils::getSaveFilePath(const std::string& gameName, const std::string& fileName)
{
	fs::path saveDir;
#ifdef _WIN32
    char* appData = nullptr;
    size_t len = 0;
    if (_dupenv_s(&appData, &len, "APPDATA") == 0 && appData != nullptr)
    {
        saveDir = fs::path(appData) / gameName;
        free(appData);
    }
    else {
        saveDir = fs::current_path() / gameName;
    }
#else
	if (const char* home = std::getenv("HOME"))
	{
		saveDir = fs::path(home) / ("." + gameName);
	}
	else {
		saveDir = fs::current_path() / gameName;
	}
#endif

	if (!fs::exists(saveDir))
		fs::create_directories(saveDir);

	return (saveDir / fileName).string();
}