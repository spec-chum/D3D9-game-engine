#ifndef _clog_h
#define _clog_h

#include "base.h"

class CLog
{
	char m_ErrorMessage[1024];

public:	
	std::ofstream LogFile;

	CLog();
	~CLog();	
};

extern CLog Log;

#endif