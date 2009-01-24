/* Logger.h */
#ifndef ROBO_LOGGER_H__
#define ROBO_LOGGER_H__

#include <string>
#include <fstream>

#include "WPILib.h"



class Logger
{
public:
	
	enum DebuggingLevel { None, Normal, Verbose, FullDebug };	
	Logger();
	
	/** Opens a log file for writing. Will replace a file by the same name. 
	 	returns true if file open is success, false otherwise */
	bool OpenFile(std::string filename);
	
	/** Closes a file. Returns true on success, false otherwise.*/
	bool CloseFile();
	
	
	/** Returns true if file is open. False otherwise. **/
	bool IsOpen() const;
	
	void Debug(std::string msg);
	
	void Info(std::string msg);
	
	void SetDebugLevel(DebuggingLevel dbgLvl);
	
	DebuggingLevel DebugLevel() const;
private:
	
	//void ToDashboard(std::string msg);
	DebuggingLevel dbgLvl;
	std::ofstream out;
	//Dashboard &dash;
	
};

#endif
