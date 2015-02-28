#include "log.h"

CLog Log;

CLog::CLog()
{	
	LogFile.open("LogFile.txt");
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);

	LogFile	<< std::setfill('0');	
	LogFile	<< "Log File created on " << std::setw(2) << SysTime.wDay << "/";
	LogFile	<< std::setw(2) << SysTime.wMonth << "/" << std::setw(2) << SysTime.wYear;
	LogFile	<< " at " << std::setw(2) << std::setw(2) << SysTime.wHour << ":"; 
	LogFile	<< std::setw(2) << SysTime.wMinute << "\n\n";
}

CLog::~CLog()
{
	LogFile.close();
}
