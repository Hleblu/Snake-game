#pragma once
#include <fstream>
#include <map>
#include <string>
#include "serializable.hpp"

namespace {
	constexpr char VERSION = 'B';
}

class SaveManager
{
	std::map<std::string, Serializable*> clients;
public:
	void bind(Serializable& serialized)
	{
		clients[serialized.getHeader()] = &serialized;
	}

	void resetAll()
	{
		for (auto& client : clients)
			client.second->setDefaults();
	}

	bool save(const std::string& filePath)
	{
		std::fstream file(filePath, std::ios::out | std::ios::trunc);

		if (!file.is_open())
			return false;

		file << VERSION << "\n";

		for (auto client : clients)
		{
			file << "[" << client.first << "]\n";
			Archive archive(file, true);
			client.second->serialize(archive);
			file << "\n";
		}

		return !file.fail();
	}
	bool load(const std::string& filePath)
	{
		std::fstream file(filePath, std::ios::in);

		if (!file.is_open())
			return false;

		char version;
		file >> version;
		if (version != VERSION)
			return false;

		std::string candidate;
		while (std::getline(file, candidate))
		{
			if (candidate.empty()) continue;
			std::size_t startPos = candidate.find('[');
			std::size_t endPos = candidate.find(']');

			bool isValid = startPos != std::string::npos && endPos != std::string::npos && startPos < endPos;
			if (!isValid) continue;

			size_t length = endPos - startPos - 1;
			const std::string key = candidate.substr(startPos + 1, length);

			auto it = clients.find(key);
			if (it != clients.end())
			{
				try {
					Archive archive(file, false);
					it->second->serialize(archive);
				}
				catch (const std::runtime_error& e) {
					resetAll();
					return false;
				}
			}
		}

		if (file.fail() && !file.eof()) {
			resetAll();
			return false;
		}

		return true;
	}
};