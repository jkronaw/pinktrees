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

	ShaderCompilationException::ShaderCompilationException(const char* filename)
	{
		std::stringstream ss;
		ss << "Shader could not be compiled. Log output: \n" << filename;
		message = ss.str();
	}

	ShaderProgramLinkageException::ShaderProgramLinkageException(const char* filename)
	{
		std::stringstream ss;
		ss << "Shader program could not be linked. Log output: \n" << filename;
		message = ss.str();
	}
}