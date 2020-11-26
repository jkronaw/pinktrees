#include <string>

#include <istream>

namespace engine
{
	struct Exception
	{
		Exception() = default;
		explicit Exception(const char* message);
		explicit Exception(std::string message);
		std::string message;
	};

	struct FileCouldNotBeOpenedException : Exception
	{
		explicit FileCouldNotBeOpenedException(const char* message);
	};

	struct MatrixNotInvertibleException : Exception
	{
		MatrixNotInvertibleException() = default;
	};
}
