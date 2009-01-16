#include <string>
#include <fstream>

#include "Logger.h"

using std::ios_base;
using std::ofstream;
using std::string;

Logger::Logger(void)
	: out()
{
}

bool Logger::OpenFile(std::string filename)
{
	out.open(filename.c_str(), std::ios_base::out);
	//out.open(filename, std::ios_base::oiut)
	
	return out.is_open();
}

bool Logger::CloseFile(void)
{
	out.close();
	
	return false;
}
