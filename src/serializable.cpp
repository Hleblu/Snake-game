#include "serializable.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

Archive::Archive(std::iostream& stream, bool isSaving) :
	stream(stream), isSaving(isSaving)
{
	if (isSaving)
		stream
		<< std::setprecision(std::numeric_limits<float>::max_digits10)
		<< std::boolalpha;
	else
		stream >> std::boolalpha;
}

Archive& Archive::operator& (std::string& data)
{
	if (isSaving)
		stream << std::quoted(data) << ' ';
	else
		stream >> std::quoted(data);

	return *this;
}