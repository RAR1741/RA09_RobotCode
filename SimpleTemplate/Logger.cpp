#include "WPILib.h"
#include "Logger.h"

using std::ios_base;
using std::ofstream;
using std::string;

Logger::Logger(void)
	: out()
	//dash(DriverStation::GetInstance()->GetDashboardPacker())
{
	//ToDashboard("Logging system initialized. Beeewooop.");
}


void Logger::Debug(std::string msg)
{
	if (!IsOpen() || DebugLevel() < Logger::FullDebug  ) return;	// Don't write anything if the file isn't open.
	out << msg << std::endl;
}

void Logger::Info(std::string msg)
{
	if (!IsOpen() || DebugLevel() < Logger::Verbose) return;
	out << msg << std::endl;
}

void Logger::SetDebugLevel(DebuggingLevel dbgLvl)
{
	this->dbgLvl = dbgLvl;
}

enum Logger::DebuggingLevel Logger::DebugLevel() const
{
	return dbgLvl;
}

inline bool Logger::IsOpen(void) const 
{
	return out.is_open();	// Hopefully this will be made an inline function
}
/*
void Logger::ToDashboard(std::string msg)
{
	//dash.Printf("Log: %s", msg.c_str());
	
	//dprintf("Log: %s", msg.c_str());
}
*/

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
