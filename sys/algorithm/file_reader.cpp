#include "file_reader.h"
#include <fstream>
#include <error/exceptions.h>

namespace sys
{

sys::string read_file(const sys::string& file)
{
	std::string cvtfile = convert<char, sys::char_t>(file);

	std::ifstream file_stream(cvtfile);
	if (file_stream.is_open() == false)
	{
		throw exception_t(E_COULD_NOT_OPEN_FILE);
	}

	std::string output;
	output.assign((std::istreambuf_iterator<char>(file_stream)),
		(std::istreambuf_iterator<char>()));

	return convert<sys::char_t, char>(output);
}

}