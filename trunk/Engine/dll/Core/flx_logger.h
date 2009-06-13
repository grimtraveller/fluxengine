/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef Logger_H
#define Logger_H

#include "flx_core.h"
#include "flx_singleton.h"
#include <iostream>
#include <fstream>
#include <time.h>

enum LOGENTRY_TYPE 
{
	LOG_DEFAULT = 0,
	LOG_ERROR,
	LOG_WARNING
};

static std::string LOGENTRY_TYPE_STR[3] =
{
	"",
	"Error: ",
	"Warning :"
};

class ENGINE_API Logger : public Singleton<Logger>
{
public:
	Logger();
	virtual ~Logger();

	void			Write(LOGENTRY_TYPE log_entry_type, const char* szMessage, ...);
	LOGENTRY_TYPE	GetErrorCode();
	char*			GetErrorMessage();

	void			ShowLogMessage();

protected:

private:
	char			m_szFilename[260];
	char			m_szLogMsg[1024];
	LOGENTRY_TYPE	m_log_entry_id;

	bool			m_bON;
};

#define FLX_LOGGER Logger::getInstance()

#endif