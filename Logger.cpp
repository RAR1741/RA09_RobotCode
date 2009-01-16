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


void Logger::Debug(std::string msg)
{
	if (!IsOpen()) return;	// Don't write anything if the file isn't open.
	out << msg << std::endl;
}
inline bool Logger::IsOpen(void) const {
	return out.is_open();	// Hopefully this will be made an inline function
}

bool Logger::OpenFile(std::string filename)
{
	out.open(filename.c_str(), std::ios_base::out);
	//out.open(filename, std::ios_base::oiut)
	
	return IsOpen();
}

bool Logger::CloseFile(void)
{
	out.close();
	
	return !IsOpen();
}
