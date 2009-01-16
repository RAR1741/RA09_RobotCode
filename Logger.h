/* Logger.h */
#ifndef LOGGER_H__
#define LOGGER_H__

#include <string>
#include <fstream>

class Logger
{
public:
	Logger();
	
	bool OpenFile(std::string filename);
	bool CloseFile();
	
	void Debug(std::string msg)
	{
		out << msg << std::endl;
	}
private:
	std::ofstream out;
};

#endif
