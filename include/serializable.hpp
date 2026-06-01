#pragma once
#include <istream>
#include <string>

class Archive
{
	std::iostream& stream;
	bool isSaving;
	bool isGood;
public:
	Archive(std::iostream& stream, bool isSaving);

	template<typename T> Archive& operator& (T& data)
	{
		if (isSaving)
			stream << data << ' ';
		else {
			if (isGood == false) return *this;
			if (!(stream >> data)) isGood = false;
		}

		return *this;
	}

	Archive& operator& (std::string& data);
};

class Serializable
{
public:
	virtual ~Serializable() = default;
	virtual std::string getHeader() const = 0;
	virtual void serialize(Archive& archive) = 0;
};