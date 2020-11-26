#include "exceptions.h"

#include <sstream>

namespace engine
{
	Exception::Exception(const char* message) : message(message) {}
	Exception::Exception(std::string message) : message(message) {}

	FileCouldNotBeOpenedException::FileCouldNotBeOpenedException(const char* filename)
	{
		std::stringstream ss;
		ss << "File '" << filename << "' could not be openend.";
		message = ss.str();
	}
}