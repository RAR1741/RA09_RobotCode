/* Logger.h */
#ifndef LOGGER_H__
#define LOGGER_H__

#include <string>
#include <fstream>

class Logger
{
public:
	Logger();
	
	/** Opens a log file for writing. Will replace a file by the same name. 
	 	returns true if file open is success, false otherwise */
	bool OpenFile(std::string filename);
	
	/** Closes a file. Returns true on success, false otherwise.*/
	bool CloseFile();
	
	
	/** Returns true if file is open. False otherwise. **/
	bool IsOpen() const;
	
	void Debug(std::string msg);
private:
	std::ofstream out;
};

#endif
